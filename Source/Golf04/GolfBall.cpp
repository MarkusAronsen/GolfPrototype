// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfBall.h"



// Sets default values
AGolfBall::AGolfBall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"), true);
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"), true);
	mCollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"), true);
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"), true);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("/Game/StaticMesh/BaseGolfMesh.BaseGolfMesh"));
	if (FoundMesh.Succeeded())
		mMesh->SetStaticMesh(FoundMesh.Object);
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find base mesh for player character."));
	RootComponent = mMesh;
	mCollisionBox->SetupAttachment(mMesh);
	mSpringArm->SetupAttachment(RootComponent);
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

	mMesh->SetLinearDamping(0.6f);
	mMesh->SetAngularDamping(0.3f);

	mMesh->BodyInstance.SetMassOverride(100.f);
	mMesh->BodyInstance.bEnableGravity = true;

	mMesh->SetSimulatePhysics(true);
	mMesh->BodyInstance.bUseCCD = true;



	//mCollisionBox->SetupAttachment(mMesh);

	mCollisionBox->SetSphereRadius(45.f);

	mCollisionBox->SetLinearDamping(0.1f);
	mCollisionBox->SetAngularDamping(0.1f);

	mCollisionBox->BodyInstance.SetMassOverride(100.f);
	mCollisionBox->BodyInstance.bEnableGravity = true;

	mCollisionBox->SetWorldScale3D(FVector(1.75f, 1.75f, 1.75f));


	//mSpringArm->SetupAttachment(RootComponent);

	mSpringArm->RelativeRotation = FRotator(-30.f, 0.f, 0.f);

	mSpringArm->TargetArmLength = 500.f;

	mSpringArm->bEnableCameraLag = true;
	mSpringArm->bEnableCameraRotationLag = true;
	mSpringArm->CameraRotationLagSpeed = 10.f;
	mSpringArm->CameraLagSpeed = 10.f;
	mSpringArm->CameraLagMaxDistance = 100.f;

	mSpringArm->bUsePawnControlRotation = true;
	mSpringArm->bInheritPitch = false;
	mSpringArm->bInheritYaw = true;
	mSpringArm->bInheritRoll = false;

	mCamera->SetRelativeRotation(FRotator(15.f, 0, 0));

}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();

	if (mCollisionBox)
	{
		mCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGolfBall::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player character no collision box"));
	}

	if (PowerBarWidget_BP)
	{
		PowerBarWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PowerBarWidget_BP);
		if (PowerBarWidget)
			PowerBarWidget->AddToViewport();
	}

	walkMaxDuration = 30.f;

	world = GetWorld();

	state = GOLF;

}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	onGround = sphereTrace();

	switch (state)
	{
	case GOLF:
		if (currentLaunchPower > maxLaunchPower)
			currentLaunchPower = maxLaunchPower;
		else if (LMBPressed)
			currentLaunchPower = currentLaunchPower + launchPowerIncrement;

		if (mMesh->GetPhysicsLinearVelocity().Size() < 100.f)
			canLaunch = true;
		else
			canLaunch = false;


		break;
	case WALKING:
		break;
	case CLIMBING:
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
		mCamera->SetWorldLocation(climbingCameraPosition * 1000.f + GetActorLocation());

		break;
	case FLYING:
		mCamera->SetComponentToWorld(
			FTransform(GetActorRotation() + FRotator(0.f, -90.f, 0.f),
				GetActorLocation() + (GetActorForwardVector().RotateAngleAxis(90.f, FVector(0.f, 0.f, 1.f)) * 2000.f,
					FVector::OneVector)));
		break;
	};
}

// Called to bind functionality to input
void AGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//InputComponent->BindAction("Spacebar", IE_Pressed, this, &AGolfBall::launchReady);
	InputComponent->BindAction("Spacebar", IE_Pressed, this, &AGolfBall::flappyAscend);
	InputComponent->BindAction("W", IE_Pressed, this, &AGolfBall::setW);
	InputComponent->BindAction("W", IE_Released, this, &AGolfBall::setW);
	InputComponent->BindAction("A", IE_Pressed, this, &AGolfBall::setA);
	InputComponent->BindAction("A", IE_Released, this, &AGolfBall::setA);
	InputComponent->BindAction("S", IE_Pressed, this, &AGolfBall::setS);
	InputComponent->BindAction("S", IE_Released, this, &AGolfBall::setS);
	InputComponent->BindAction("D", IE_Pressed, this, &AGolfBall::setD);
	InputComponent->BindAction("D", IE_Released, this, &AGolfBall::setD);

	InputComponent->BindAction("Left Mouse Button", IE_Pressed, this, &AGolfBall::setLMBPressed);
	InputComponent->BindAction("Left Mouse Button", IE_Released, this, &AGolfBall::setLMBReleased);
}

void AGolfBall::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AGoal::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT GOAL"));
		UGameplayStatics::OpenLevel(GetWorld(), "Level0");
	}
	if (OtherActor->IsA(ALegsPUp::StaticClass()))
	{

		state = WALKING;
		walkTimer = walkMaxDuration;
		OtherActor->Destroy();
	}
	if (OtherActor->IsA(AClimbObject::StaticClass()))
	{
		state = CLIMBING;
		world->GetFirstPlayerController()->bShowMouseCursor = true;
		LockedClimbRotation = GetActorRotation();
		LockedClimbPosition = GetActorLocation();
		climbingCanLaunch = true;
		mMesh->SetSimulatePhysics(false);

		mCamera->AttachTo(RootComponent, NAME_None, EAttachLocation::KeepWorldPosition);
		climbingCameraPosition = OtherActor->GetActorLocation().ForwardVector;
		climbingCameraRotation = OtherActor->GetActorRotation() + FRotator(0.f, 180.f, 0.f);
		mCamera->SetRelativeRotation(climbingCameraRotation);
	}

	if (OtherActor->IsA(AWingsPUp::StaticClass()))
	{
		state = FLYING;
		mMesh->SetSimulatePhysics(false);
		OtherActor->Destroy();
	}
}

void AGolfBall::walkFunction(float deltaTime)
{
	if (walkTimer < 0)
	{
		state = GOLF;
		return;
	}
	walkTimer = walkTimer - deltaTime;
}

void AGolfBall::flying(float deltaTime)
{
	Acceleration = Acceleration - 0.4f;
	FlyingVector = FVector(0.f, -7.f, Gravity + Ascend + Acceleration);
	SetActorLocation(GetActorLocation() + FlyingVector * 100 * deltaTime);
	if (Ascend > 0.f)
		Ascend = Ascend - 3.f;
}

void AGolfBall::flappyAscend()
{
	if (state == FLYING)
	{
		Ascend = 40.f;
		Acceleration = 0.f;
	}
}

void AGolfBall::setW()
{
	WPressed = !WPressed;
}

void AGolfBall::setA()
{
	APressed = !APressed;
}

void AGolfBall::setS()
{
	SPressed = !SPressed;
}

void AGolfBall::setD()
{
	DPressed = !DPressed;
}

void AGolfBall::setLMBPressed()
{
	LMBPressed = true;
	if (state == CLIMBING && !mMesh->IsSimulatingPhysics())
		world->GetFirstPlayerController()->DeprojectMousePositionToWorld(mousePositionClicked, oneDirection);
}
void AGolfBall::setLMBReleased()
{
	LMBPressed = false;

	if (state == GOLF)
	{
		mMesh->SetPhysicsLinearVelocity(FRotator(0.f, GetControlRotation().Yaw, 0.f).Vector() * currentLaunchPower, true);
		currentLaunchPower = 0.f;
	}

	if (state == CLIMBING && !mMesh->IsSimulatingPhysics())
	{
		world->GetFirstPlayerController()->DeprojectMousePositionToWorld(mousePositionReleased, oneDirection);
		mMesh->SetSimulatePhysics(true);
		UE_LOG(LogTemp, Warning, TEXT("%f,   %f,   %f"), mousePositionClicked.X, mousePositionClicked.Y, mousePositionClicked.Z);
		UE_LOG(LogTemp, Warning, TEXT("%f,   %f,   %f"), mousePositionReleased.X, mousePositionReleased.Y, mousePositionReleased.Z);
		mMesh->SetPhysicsLinearVelocity((mousePositionReleased - mousePositionClicked) * 1000.f, false);
	}
}

bool AGolfBall::sphereTrace()
{
	DrawDebugSphere(GetWorld(), mMesh->GetComponentToWorld().GetLocation(), mCollisionBox->GetCollisionShape().Sphere.Radius, 32, FColor::Cyan);

	if (world && mMesh)
		world->SweepMultiByChannel(
			hitResults,
			mMesh->GetComponentToWorld().GetLocation(),
			mMesh->GetComponentToWorld().GetLocation() - FVector(0, 0, 50),
			FQuat::Identity,
			ECC_Visibility,
			mCollisionBox->GetCollisionShape());


	return hitResults.Num() > 2;
}