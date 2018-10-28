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


	//UE_LOG(LogTemp, Warning, TEXT("Rotation: %s"), *GetActorRotation().ToString());
}

// Called to bind functionality to input
void AGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

		/*Mesh->BodyInstance.bLockXRotation = true;
		Mesh->BodyInstance.bLockYRotation = true;
		Mesh->BodyInstance.bLockZRotation = true;*/
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
