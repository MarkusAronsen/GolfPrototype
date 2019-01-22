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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundMesh(TEXT("/Game/Models/low_poly_golfball.low_poly_golfball"));
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

	mMesh->BodyInstance.bEnableGravity = true;

	mMesh->SetSimulatePhysics(true);
	mMesh->BodyInstance.bUseCCD = true;



	//mCollisionBox->SetupAttachment(mMesh);
	mCollisionBox->SetSphereRadius(45.f);
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
	debugV = FVector(1000.f, 0.f, 50.f);
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

		break;

	case FLYING:
		
		break;
	};

	if(world)
		drawDebugObjectsTick();
	debugMouse();

	DrawDebugLine(world, FVector(0.f, 0.f, 50.f), debugV, FColor::Red, true, 3.f, 100.f);
	debugV = debugV.RotateAngleAxis(1.f, FVector(0.f, 0.f, 1.f));
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
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &AGolfBall::scrollUp);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &AGolfBall::scrollDown);

	InputComponent->BindAction("Left Mouse Button", IE_Pressed, this, &AGolfBall::setLMBPressed);
	InputComponent->BindAction("Left Mouse Button", IE_Released, this, &AGolfBall::setLMBReleased);
}

void AGolfBall::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AGoal::StaticClass()))
	{
		UGameplayStatics::OpenLevel(GetWorld(), "GOFF2");
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
		mMesh->SetSimulatePhysics(false);
		world->GetFirstPlayerController()->bShowMouseCursor = true;
		SetActorLocation(OtherActor->GetActorLocation() + OtherActor->GetActorForwardVector() * 50.f);
		OActorForwardVector = OtherActor->GetActorForwardVector();
		SetActorRotation(FRotator(0.f, OtherActor->GetActorRotation().Yaw + 180.f, 0.f));

		mSpringArm->bInheritYaw = false;
		mSpringArm->CameraLagSpeed = 5.f;
		mSpringArm->SetRelativeRotation(GetActorRotation());
		mSpringArm->TargetArmLength = 1500.f;
		mCamera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	}

	if (OtherActor->IsA(AWingsPUp::StaticClass()))
	{
		state = FLYING;
		mMesh->SetSimulatePhysics(false);
		SetActorLocation(OtherActor->GetActorLocation());
		SetActorRotation(OtherActor->GetActorRotation());

		mSpringArm->bInheritYaw = false;
		mSpringArm->SetRelativeRotation(GetActorRightVector().Rotation() + FRotator(0.f, 180.f, 0.f));
		mSpringArm->TargetArmLength = 2000.f;
		mCamera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
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
		{
			mousePositionClicked = FVector(0.f, mouseX, mouseY);
		}
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
			mousePositionReleased = FVector(0.f, mouseX, mouseY);
			mMesh->SetSimulatePhysics(true);
			mousePositionReleased = mousePositionReleased - mousePositionClicked;
			mousePositionReleased = mousePositionReleased.RotateAngleAxis(OActorForwardVector.Rotation().Yaw, FVector(0, 0, 1));
			DrawDebugLine(world, GetActorLocation(), GetActorLocation() + mousePositionReleased, FColor::Blue, true, 15.f);
			mMesh->SetPhysicsLinearVelocity(mousePositionReleased * 5.f, false);
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
	state = WALKING;
	walkTimer = walkMaxDuration;
}

void AGolfBall::scrollUp()
{
	if (mSpringArm->TargetArmLength > 500.f)
		mSpringArm->TargetArmLength -= 100.f;
}

void AGolfBall::scrollDown()
{
	if (mSpringArm->TargetArmLength < 2000.f)
		mSpringArm->TargetArmLength += 100.f;
}

bool AGolfBall::sphereTrace()
{
	//if(world)
	//DrawDebugSphere(GetWorld(), mMesh->GetComponentToWorld().GetLocation(), mCollisionBox->GetCollisionShape().Sphere.Radius, 32, FColor::Cyan);

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

void AGolfBall::debugMouse()
{
	world->GetFirstPlayerController()->GetMousePosition(mouseX, mouseY);
	debugMouseX = FString::SanitizeFloat(mouseX);
    debugMouseY = FString::SanitizeFloat(572.f - mouseY);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("Mouse X: " + debugMouseX + "\n Mouse Y: " + debugMouseY));
}

void AGolfBall::drawDebugObjectsTick()
{
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetForwardVector() * 200, FColor::Red, false, 0, (uint8)'\000', 6.f);
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetUpVector() * 200, FColor::Green, false, 0, (uint8)'\000', 6.f);
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetRightVector() * 200, FColor::Blue, false, 0, (uint8)'\000', 6.f);


	/*FVector DebugPosition;
	FVector DebugDirection;

	world->GetFirstPlayerController()->DeprojectMousePositionToWorld(DebugPosition, DebugDirection);

	FVector DebugIntersection;
	DebugIntersection = FMath::LinePlaneIntersection(
		DebugPosition,
		GetActorLocation() + DebugDirection * 5000,
		GetActorLocation(),
		GetActorForwardVector().RotateAngleAxis(90.f, FVector(0.f, 1.f, 0.f)));

	DrawDebugSphere(GetWorld(), DebugIntersection, 20, 10, FColor::Purple, false, 1);*/

	/*UKismetSystemLibrary::DrawDebugPlane(
	this,
	FPlane(GetActorLocation(), GetActorForwardVector().RotateAngleAxis(90.f, FVector(0.f, 1.f, 0.f))),
	GetActorLocation(),
	200,
	FLinearColor::Blue,
	0.1f); */

	//DrawDebugLine(GetWorld(), mousePositionClickedAfterTrace, mousePositionReleasedAfterTrace, FColor::Emerald, true, 1.f, (uint8)'\000', 6.f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorForwardVector().RotateAngleAxis(90.f, FVector(0.f, 1.f, 0.f)), FColor::Magenta, true, 1.f, (uint8)'\000', 6.f);


	//FPlane debug = FPlane(FVector(0, 0, 100), FVector(3000, 0, 100), FVector(0, 540, 100));

	//UKismetSystemLibrary::DrawDebugPlane(this, debug, GetActorLocation(), 100, FLinearColor::Blue, 1);
}
