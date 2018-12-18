// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfBall.h"



// Sets default values
AGolfBall::AGolfBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<USphereComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGolfBall::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character no collision box"));
	}

	Mesh = this->FindComponentByClass<UStaticMeshComponent>();

	if (!Mesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh not found"));
	}

	RootComponent = Mesh;

	/*Mesh->BodyInstance.bLockRotation = true;
	Mesh->BodyInstance.CreateDOFLock();*/

	walkMaxDuration = 30.f;

	world = GetWorld();
}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isWalking)
	{
		walkFunction(DeltaTime);
	}

	if (isClimbing)
	{
		SetActorRotation(LockedClimbRotation);
	}
	if (climbingCanLaunch)
	{
		SetActorLocation(LockedClimbPosition);
	}
	if (isSomersaulting)
	{
		orbit();
		if (toLaunch)
			launchBall();
		PrevPos = GetActorLocation();
	}
	if (isFlying)
	{
		flying(DeltaTime);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *GetActorRotation().ToString());
}

// Called to bind functionality to input
void AGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Spacebar", IE_Pressed, this, &AGolfBall::launchReady);
	InputComponent->BindAction("Spacebar", IE_Pressed, this, &AGolfBall::flappyAscend);

}

void AGolfBall::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AGoal::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT GOAL"));
		UGameplayStatics::OpenLevel(world, "GOFF2");
	}
	if (OtherActor->IsA(ALegsPUp::StaticClass()))
	{

		isWalking = true;
		walkTimer = walkMaxDuration;
		OtherActor->Destroy();
	}
	if (OtherActor->IsA(AClimbObject::StaticClass()))
	{

		isClimbing = true;

		LockedClimbRotation = GetActorRotation();
		LockedClimbPosition = GetActorLocation();
		climbingCanLaunch = true;
		Mesh->SetSimulatePhysics(false);

		/*Mesh->BodyInstance.bLockXRotation = true;
		Mesh->BodyInstance.bLockYRotation = true;
		Mesh->BodyInstance.bLockZRotation = true;*/
	}
	if (OtherActor->IsA(ASomersaultObject::StaticClass()))
	{
		isSomersaulting = true;
		Mesh->SetSimulatePhysics(false);
		SomersaultCenter = OtherActor->GetActorLocation();
		SetActorLocation(OtherActor->GetActorLocation() + FVector(0, 0, -100));
	}

	if (OtherActor->IsA(AWingsPUp::StaticClass()))
	{
		isFlying = true;
		Mesh->SetSimulatePhysics(false);
		OtherActor->Destroy();
	}
}

void AGolfBall::walkFunction(float deltaTime)
{
	if (walkTimer < 0)
	{
		isWalking = false;
		return;
	}
	walkTimer = walkTimer - deltaTime;
}

void AGolfBall::launchBall()
{
	isSomersaulting = false;
	toLaunch = false;
	Mesh->SetSimulatePhysics(true);
	Mesh->SetPhysicsLinearVelocity((GetActorLocation() - PrevPos) * 400);

}

void AGolfBall::orbit()
{
	if (degree > PI * 1.5 + 2 * PI)
		degree = PI * 1.5;
	SetActorLocation(SomersaultCenter + FVector(0.f, cos(degree) * radius, sin(degree) * radius));
	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SomersaultCenter));
	degree += 0.05f;
}

void AGolfBall::launchReady()
{
	if(isSomersaulting)
		toLaunch = true;
}

void AGolfBall::flying(float deltaTime)
{
	Acceleration = Acceleration - 0.4f;
	FlyingVector = FVector(0.f, -7.f, Gravity + Ascend + Acceleration);
	SetActorLocation(GetActorLocation() + FlyingVector * 100 * deltaTime);
	if (Ascend > 0.f)
	{
		Ascend = Ascend - 3.f;
	}
}

void AGolfBall::flappyAscend()
{
	if (isFlying)
	{
		Ascend = 40.f;
		Acceleration = 0.f;
	}
}
