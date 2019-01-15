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
		{
			PowerBarWidget->AddToViewport();
			PowerBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	walkMaxDuration = 30.f;
	movementSpeed = 150.f;
	world = GetWorld();

	state = WALKING;

}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	onGround = sphereTrace();

	switch (state)
	{
	case GOLF:

		mouseCameraYaw();
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

		mouseCameraYaw();
		tickWalking();
		break;
	
	case CLIMBING:

		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(mouseX, mouseY);
		DrawDebugLine(GetWorld(), GetActorLocation(), (GetActorLocation() + (mousePositionClicked - mousePositionReleased)) * 10.f, FColor(255.f, 0.f, 0.f, 1.f), true, 1.f, (uint8)'\000', 10.f);
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

	InputComponent->BindAction("Spacebar", IE_Pressed, this, &AGolfBall::spacebarPressed);
	InputComponent->BindAction("Left Shift", IE_Pressed, this, &AGolfBall::leftShiftPressed);
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
	InputComponent->BindAction("Wheel Up", IE_Pressed, this, &AGolfBall::zoomIn);
	InputComponent->BindAction("Wheel Down", IE_Pressed, this, &AGolfBall::zoomOut);
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
		mMesh->SetSimulatePhysics(false);
		climbingCanLaunch = true;

		SetActorLocation(OtherActor->GetActorLocation() + OtherActor->GetActorUpVector() * 50.f);
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), OtherActor->GetActorLocation()));
		mSpringArm->bInheritYaw = false;
		mSpringArm->CameraLagSpeed = 5.f;
		mSpringArm->SetRelativeRotation(FRotator(-170.f, 0.f, 180.f));
		mSpringArm->TargetArmLength = 1500.f;

		mMesh->BodyInstance.CustomDOFPlaneNormal = GetActorForwardVector().RotateAngleAxis(90.f, FVector(0.f, 1.f, 0.f));
		mMesh->SetConstraintMode(EDOFMode::CustomPlane);
		mMesh->BodyInstance.bLockRotation = true;
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

void AGolfBall::jump()
{
	mMesh->SetPhysicsLinearVelocity(mMesh->GetPhysicsLinearVelocity() + FVector(0, 0, 1500), true);
}

void AGolfBall::spacebarPressed()
{
	if (state == FLYING)
		flappyAscend();
	if (state == WALKING && onGround)
		jump();
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
	switch (state)
	{
	case GOLF:
		PowerBarWidget->SetVisibility(ESlateVisibility::Visible);
		break;
	case WALKING:
		break;
	case CLIMBING:
		if (!mMesh->IsSimulatingPhysics())
			world->GetFirstPlayerController()->DeprojectMousePositionToWorld(mousePositionClicked, oneDirection);
		break;
	case FLYING:
		break;
	}
}
void AGolfBall::setLMBReleased()
{
	LMBPressed = false;
	switch (state)
	{
	case GOLF:
		mMesh->SetPhysicsLinearVelocity(FRotator(0.f, GetControlRotation().Yaw, 0.f).Vector() * currentLaunchPower, true);
		currentLaunchPower = 0.f;
		PowerBarWidget->SetVisibility(ESlateVisibility::Hidden);
		break;
	case WALKING:
		break;
	case CLIMBING:
		if (!mMesh->IsSimulatingPhysics())
		{
			world->GetFirstPlayerController()->DeprojectMousePositionToWorld(mousePositionReleased, oneDirection);
			mMesh->SetSimulatePhysics(true);
			mMesh->SetPhysicsLinearVelocity((mousePositionClicked - mousePositionReleased) * 1000.f, false);
		}
		break;
	case FLYING:
		break;
	}
}

void AGolfBall::mouseCameraPitch()
{
	world->GetFirstPlayerController()->GetInputMouseDelta(mouseX, mouseY);
	world->GetFirstPlayerController()->AddPitchInput(mouseY);
}

void AGolfBall::mouseCameraYaw()
{
	world->GetFirstPlayerController()->GetInputMouseDelta(mouseX, mouseY);
	world->GetFirstPlayerController()->AddYawInput(mouseX);
}

void AGolfBall::leftShiftPressed()
{
	if(!mMesh->IsSimulatingPhysics())
		mMesh->SetSimulatePhysics(true);
	if (state != WALKING)
		state = WALKING;
	else
		state = GOLF;
	walkTimer = walkMaxDuration;
}

void AGolfBall::zoomOut()
{
	if(mSpringArm->TargetArmLength < 1500.f)
		mSpringArm->TargetArmLength += zoomSpeed;
}

void AGolfBall::zoomIn()
{
	if(mSpringArm->TargetArmLength > 300.f)
		mSpringArm->TargetArmLength -= zoomSpeed;
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

void AGolfBall::tickWalking()
{
	mMesh->SetWorldRotation(GetControlRotation());

	if (onGround && (WPressed || APressed || SPressed || DPressed))
	{
		if (APressed)
			ADirection = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw - 90, 0));
		if (DPressed)
			DDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw + 90, 0));
		if (SPressed)
			SDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)) * -1;
		if (WPressed)
			WDirection = UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0));

		FVector Direction = ADirection + DDirection + SDirection + WDirection;
		Direction.Normalize();
		Direction *= movementSpeed;

		mMesh->SetPhysicsLinearVelocity(Direction, true);

		ADirection = FVector::ZeroVector;
		DDirection = FVector::ZeroVector;
		SDirection = FVector::ZeroVector;
		WDirection = FVector::ZeroVector;

		if (mMesh->GetPhysicsLinearVelocity().Size() >= 1500)
			mMesh->SetPhysicsLinearVelocity(mMesh->GetPhysicsLinearVelocity() * 0.9f);
	}
	else if (onGround)
	{
		FVector newVelocity = mMesh->GetPhysicsLinearVelocity();
		mMesh->SetPhysicsLinearVelocity(FVector(newVelocity.X * 0.9f, newVelocity.Y * 0.9f, newVelocity.Z));
	}

}