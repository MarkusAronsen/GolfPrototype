// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfBall.h"
#include "LevelSelecter.h"



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

	mWingsMeshLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WingsMeshLeft"), true);
	mWingsMeshRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WingsMeshRight"), true);
	mLegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegsMesh"), true);
	//mArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"), true);

	topDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"), true);
	topDownCamera->SetWorldRotation(FRotator(-90, 0, 0));

	RootComponent = mMesh;
	mCollisionBox->SetupAttachment(mMesh);
	mSpringArm->SetupAttachment(mMesh);
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);

	mWingsMeshLeft->SetupAttachment(mMesh);
	mWingsMeshRight->SetupAttachment(mMesh);
	mLegsMesh->SetupAttachment(mMesh);

	UE_LOG(LogTemp, Warning, TEXT("Golf ball constructed"));
}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();

	UStaticMesh* loadedPlayerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/low_poly_golfball.low_poly_golfball"));
	mMesh->SetStaticMesh(loadedPlayerMesh);

	USkeletalMesh* loadedLeftWingMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Models/Wings/WingsSkeletalMesh.WingsSkeletalMesh"));
	USkeletalMesh* loadedRightWingMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Models/Wings/WingsSkeletalMesh.WingsSkeletalMesh"));
	mWingsMeshLeft->SetSkeletalMesh(loadedLeftWingMesh);
	mWingsMeshRight->SetSkeletalMesh(loadedRightWingMesh);

	USkeletalMesh* loadedLegsMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/Models/Feet/FeetSkeletalMesh.FeetSkeletalMesh"));
	mLegsMesh->SetSkeletalMesh(loadedLegsMesh);

	mMesh->SetLinearDamping(0.6f);
	mMesh->SetAngularDamping(0.1f);

	mMesh->BodyInstance.bEnableGravity = true;

	mMesh->SetSimulatePhysics(true);
	mMesh->BodyInstance.bUseCCD = true;

	mCollisionBox->SetSphereRadius(35.f);
	mCollisionBox->SetWorldScale3D(FVector(1.75f, 1.75f, 1.75f));

	if (mCollisionBox)
		mCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGolfBall::OnBeginOverlap);

	else
		UE_LOG(LogTemp, Warning, TEXT("Player character no collision box"));

	if (PowerBarWidget_BP)
	{
		PowerBarWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PowerBarWidget_BP);
		if (PowerBarWidget)
		{
			PowerBarWidget->AddToViewport();
			PowerBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("PowerBarWidget not initialized"));

	walkMaxDuration = 30.f;
	world = GetWorld();

	mController = GetWorld()->GetFirstPlayerController();
	GetWorld()->GetFirstPlayerController()->ClientSetCameraFade(true, FColor::Black, FVector2D(1.1f, 0.f), cameraFadeTimer);

	UGolfSaveGame* SaveGameInstance = Cast<UGolfSaveGame>
		(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));

	if (!UGameplayStatics::DoesSaveGameExist(SaveGameInstance->slotName, SaveGameInstance->userIndex))
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);

	traceParams.bFindInitialOverlaps = false;
	traceParams.bIgnoreBlocks = false;
	traceParams.bIgnoreTouches = true;
	traceParams.bReturnFaceIndex = true;
	traceParams.bReturnPhysicalMaterial = false;
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(GetUniqueID());
	traceParams.AddIgnoredComponent(mMesh);
	traceParams.AddIgnoredComponent(mCollisionBox);

	if (mLegsMesh && mWingsMeshLeft && mWingsMeshRight)
	{
		//Disable visibility on meshes not relevant for golfing
		mLegsMesh->SetVisibility(false);
		mWingsMeshLeft->SetVisibility(false);
		mWingsMeshRight->SetVisibility(false);
		//-

		//Flip left wing to create right wing
		mWingsMeshRight->SetRelativeScale3D(FVector(1.f, -1.f, 1.f));
		//-

		//Reposition legs
		mLegsMesh->SetRelativeRotation(FRotator(0, -90, 0));
		mLegsMesh->SetRelativeLocation(FVector(0, 0, -150));
		//-
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("mLegMesh || mWingsMeshLeft || mWingsMeshRight not initialized"));
	
	//Start camera pan if level is not LevelSelect
	if (!UGameplayStatics::GetCurrentLevelName(this).Compare("LevelSelect", ESearchCase::IgnoreCase) == 0)
	{
		bCameraShouldPan = true;
		UE_LOG(LogTemp, Warning, TEXT("Start camera pan"));
	}

	cameraSpeed = Cast<UGolfGameInstance>(GetGameInstance())->cameraSpeed;

	//Load animations and use them if they exist
	UAnimBlueprint* flyingAnim = LoadObject<UAnimBlueprint>(nullptr, TEXT("AnimBlueprint'/Game/Models/Wings/FlyingAnim.FlyingAnim'"));
	UAnimBlueprint* walkAnim = LoadObject<UAnimBlueprint>(nullptr, TEXT("AnimBlueprint'/Game/Models/Feet/WalkingAnimation.WalkingAnimation'"));

	if (flyingAnim)
	{
	mWingsMeshLeft->SetAnimInstanceClass(flyingAnim->GetAnimBlueprintGeneratedClass());
	mWingsMeshRight->SetAnimInstanceClass(flyingAnim->GetAnimBlueprintGeneratedClass());
	}
	else
	UE_LOG(LogTemp, Warning, TEXT("Could not find flying animation"));

	if (walkAnim)
	{
	mLegsMesh->SetAnimInstanceClass(walkAnim->GetAnimBlueprintGeneratedClass());
	}
	else
	UE_LOG(LogTemp, Warning, TEXT("Could not find walking animation"));
	//-

	state = GOLF;
	if (mMesh)
	{
		golfInit();
		mMesh->SetEnableGravity(false);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("mMesh not initialized"));

	GEngine->SetMaxFPS(60.f);

	UE_LOG(LogTemp, Warning, TEXT("Golf ball initialized"));
}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	onGround = sphereTrace();
	alignWithSurface = lineTrace();

	switch (state)
	{
	case GOLF:
		mMesh->AddForce(gravitation * DeltaTime, NAME_None, true);
		lerpPerspective(FRotator(-30.f, 0.f, 0.f), 1000.f, FRotator(10.f, 0.f, 0.f), DeltaTime);
		mouseCameraPitch();
		mouseCameraYaw();
		if (currentLaunchPower > maxLaunchPower)
			currentLaunchPower = maxLaunchPower;
		else if (LMBPressed && canLaunch)
			currentLaunchPower = currentLaunchPower + launchPowerIncrement * DeltaTime;

		if (mMesh->GetPhysicsLinearVelocity().Size() < 100.f)
			canLaunch = true;
		else
			canLaunch = false;
		break;

	case WALKING:
		lerpPerspective(FRotator(-30.f, 0.f, 0.f), 1000.f, FRotator(10.f, 0.f, 0.f), DeltaTime);
		mouseCameraPitch();
		mouseCameraYaw();
		tickWalking(DeltaTime);
		mMesh->AddForce(gravitation * DeltaTime, NAME_None, true);

		currentRotation = FMath::Lerp(
			GetActorRotation(),
			FRotator(
				newRotationTransform.Rotator().Pitch,
				mController->GetControlRotation().Yaw + walkingDirection,
				newRotationTransform.Rotator().Roll),
			lerpTime * DeltaTime);

		break;

	case CLIMBING:
		world->GetFirstPlayerController()->GetMousePosition(mouseX, mouseY);
		if(mMesh->IsSimulatingPhysics())
			mMesh->AddForce(gravitation/2 * DeltaTime, NAME_None, true);
		lerpPerspective(GetActorRotation(), 1500.f, FRotator(0.f, 0.f, 0.f), DeltaTime);
		break;

	case FLYING:
		lerpPerspective(GetActorRightVector().Rotation() + FRotator(0.f, 180.f, 0.f), 2000.f, FRotator(0.f, 0.f, 0.f), DeltaTime);
		applyForce(gravity);
		updatePosition(DeltaTime);
		break;

	case LEVEL_SELECT:
		topDownCamera->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 2500));
		break;

	case AWAITING_LEVELSELECT_INPUT:
		mMesh->AddForce(gravitation * DeltaTime, NAME_None, true);
		lerpPerspective(FRotator(-30, 0.f, 0.f), 1000.f, FRotator(10.f, 0.f, 0.f), DeltaTime);
		mouseCameraPitch();
		mouseCameraYaw();
		tickWalking(DeltaTime);
		break;
	};

	FVector debugMouseLine = FVector(0.f, mouseX, mouseY) - mousePositionClicked;
	debugMouseLine = debugMouseLine.RotateAngleAxis(OActorForwardVector.Rotation().Yaw, FVector(0, 0, 1));
	if(LMBPressed)
		DrawDebugLine(world, GetActorLocation(), GetActorLocation() + debugMouseLine, FColor::Blue, false, -1.f, (uint8)'\000', 4.f);
	
	if (world)
		drawDebugObjectsTick();
	//debugMouse();

	if(bRespawning)
		respawnAtCheckpointTick(DeltaTime);

	animationControlTick(DeltaTime);
}


// Called to bind functionality to input
void AGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Spacebar", IE_Pressed, this, &AGolfBall::spacebarPressed);
	InputComponent->BindAction("Left Shift", IE_Pressed, this, &AGolfBall::leftShiftPressed);
	InputComponent->BindAction("W", IE_Pressed, this, &AGolfBall::WClicked);
	InputComponent->BindAction("W", IE_Released, this, &AGolfBall::WReleased);
	InputComponent->BindAction("A", IE_Pressed, this, &AGolfBall::AClicked);
	InputComponent->BindAction("A", IE_Released, this, &AGolfBall::AReleased);
	InputComponent->BindAction("S", IE_Pressed, this, &AGolfBall::SClicked);
	InputComponent->BindAction("S", IE_Released, this, &AGolfBall::SReleased);
	InputComponent->BindAction("D", IE_Pressed, this, &AGolfBall::DClicked);
	InputComponent->BindAction("D", IE_Released, this, &AGolfBall::DReleased);
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &AGolfBall::scrollUp);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &AGolfBall::scrollDown);
	InputComponent->BindAction("L", IE_Pressed, this, &AGolfBall::printLoadedGame);
	InputComponent->BindAction("R", IE_Pressed, this, &AGolfBall::respawnAtCheckpoint);
	InputComponent->BindAction("Y", IE_Pressed, this, &AGolfBall::confirmLevelSelection);

	InputComponent->BindAction("Left Mouse Button", IE_Pressed, this, &AGolfBall::setLMBPressed);
	InputComponent->BindAction("Left Mouse Button", IE_Released, this, &AGolfBall::setLMBReleased);
	InputComponent->BindAction("Right Mouse Button", IE_Pressed, this, &AGolfBall::stopStrike);
}

void AGolfBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AClimbObject::StaticClass()))
	{
		lerpTimer = 0.f;
		climbingInit(OtherActor);
	}
}

void AGolfBall::levelInit()
{
	FString levelName = UGameplayStatics::GetCurrentLevelName(this);

	if (levelName == "LevelSelect")
	{
		state = LEVEL_SELECT;
	}
	else if (levelName == "Golf1")
	{
		state = GOLF;
	}
	else if (levelName == "Walking1")
	{
		state = WALKING;
	}
	else if (levelName == "Climbing1")
	{
		state = CLIMBING;
	}
	else if (levelName == "Flying1")
	{
		state = FLYING;
	}
}

void AGolfBall::golfInit()
{
	lerpTimer = 0.f;
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

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	mMesh->SetSimulatePhysics(true);

	if(state == GOLF && mMesh != nullptr && mMesh->IsValidLowLevel())
	{ 
		UE_LOG(LogTemp, Warning, TEXT("GOLF INIT"));
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector::ZeroVector;
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 70.f;
		mMesh->RecreatePhysicsState();
	}
	if (state == WALKING && mMesh != nullptr && mMesh->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("WALKING INIT"));
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector(0.f, 0.f, -30.f);
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 105.f;
		mMesh->RecreatePhysicsState();
	}

	setMeshVisibility();

}

void AGolfBall::climbingInit(AActor* OtherActor)
{
	state = CLIMBING;
	
	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector::ZeroVector;
	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 70.f;
	mMesh->RecreatePhysicsState();

	mMesh->SetSimulatePhysics(false);
	world->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	SetActorLocation(OtherActor->GetActorLocation() + OtherActor->GetActorForwardVector() * 50.f);
	OActorForwardVector = OtherActor->GetActorForwardVector();
	SetActorRotation(FRotator(0.f, OtherActor->GetActorRotation().Yaw + 180.f, 0.f));

	mSpringArm->bInheritYaw = false;
	mSpringArm->CameraLagSpeed = 5.f;

	setMeshVisibility();
}

void AGolfBall::flyingInit(AActor *OtherActor)
{
	state = FLYING;

	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector::ZeroVector;
	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 70.f;
	mMesh->RecreatePhysicsState();

	mMesh->SetSimulatePhysics(false);
	SetActorLocation(OtherActor->GetActorLocation());
	SetActorRotation(OtherActor->GetActorRotation());

	position = OtherActor->GetActorLocation();

	mSpringArm->bInheritYaw = false;

	setMeshVisibility();
}

void AGolfBall::lerpPerspective(FRotator springToRot, float springToLength, FRotator camToRot, float DeltaTime)
{
	if (lerpTimer < 0.8f)
	{
		mSpringArm->RelativeRotation = FMath::Lerp(mSpringArm->RelativeRotation, springToRot, lerpTime * DeltaTime);
		mSpringArm->TargetArmLength = FMath::Lerp(mSpringArm->TargetArmLength, springToLength, lerpTime * DeltaTime);
		mCamera->SetRelativeRotation(FMath::Lerp(mCamera->RelativeRotation, camToRot, lerpTime * DeltaTime));
		lerpTimer += DeltaTime;
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

void AGolfBall::jump()
{
	mMesh->SetPhysicsLinearVelocity(FVector(mMesh->GetPhysicsLinearVelocity().X, mMesh->GetPhysicsLinearVelocity().Y, 3000), false);
	if (onPlatform)
		platformJump = true;
}

void AGolfBall::applyForce(FVector force)
{
	acceleration += force;
}

void AGolfBall::updatePosition(float DeltaTime)
{
	velocity += acceleration;

	position += velocity;
	position += FVector(GetActorForwardVector() * 700 * DeltaTime);
	acceleration = FVector::ZeroVector;

	SetActorLocation(position);
}
void AGolfBall::stopStrike()
{
	if (currentLaunchPower > 0.f && state == GOLF)
	{ 
		LMBPressed = false;
		currentLaunchPower = 0.f;

		PowerBarWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
void AGolfBall::spacebarPressed()
{
	if (state == FLYING)
	{
		velocity = FVector::ZeroVector;
		applyForce(FVector(0.f, 0.f, 30.f));
		bRestartFlyingAnim = true;
	}
	if (state == WALKING && onGround)
		jump();
}

void AGolfBall::WClicked()
{
	WPressed = true;
}

void AGolfBall::WReleased()
{
	WPressed = false;
}

void AGolfBall::AClicked()
{
	APressed = true;
}

void AGolfBall::AReleased()
{
	APressed = false;
}

void AGolfBall::SClicked()
{
	SPressed = true;
}

void AGolfBall::SReleased()
{
	SPressed = false;
}

void AGolfBall::DClicked()
{
	DPressed = true;
}

void AGolfBall::DReleased()
{
	DPressed = false;
}

void AGolfBall::setLMBPressed()
{
	LMBPressed = true;
	switch (state)
	{
	case GOLF:
		if(canLaunch && PowerBarWidget)
			PowerBarWidget->SetVisibility(ESlateVisibility::Visible);

		break;
	case WALKING:
		break;
	case CLIMBING:
		if (!mMesh->IsSimulatingPhysics())
		{
			UE_LOG(LogTemp, Warning, TEXT("CLICKED"));
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

		if(PowerBarWidget)
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
			mMesh->SetPhysicsLinearVelocity(mousePositionReleased * 5, false);
		}
		break;
	case FLYING:
		break;
	}
}

void AGolfBall::mouseCameraPitch()
{
	world->GetFirstPlayerController()->GetInputMouseDelta(mouseX, mouseY);
	mCamera->RelativeRotation.Pitch = FMath::Clamp(mCamera->RelativeRotation.Pitch + (mouseY * cameraSpeed), -10.f, 30.f);
}

void AGolfBall::mouseCameraYaw()
{
	world->GetFirstPlayerController()->GetInputMouseDelta(mouseX, mouseY);
	world->GetFirstPlayerController()->AddYawInput(mouseX * cameraSpeed);
}

void AGolfBall::leftShiftPressed()
{
	lerpTimer = 0.f;
	if (!mMesh->IsSimulatingPhysics())
		mMesh->SetSimulatePhysics(true);

	else if (state == CLIMBING)
	{
		state = WALKING;
		golfInit();
	}
	else if (state == WALKING)
	{
		state = GOLF;
		golfInit();
	}
	else if (state == GOLF)
	{
		state = WALKING;
		golfInit();
	}


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
	if (world && mMesh)
		world->SweepMultiByChannel(
			hitResults,
			mMesh->GetComponentToWorld().GetLocation(),
			mMesh->GetComponentToWorld().GetLocation() - FVector(0, 0, 120),
			FQuat::Identity,
			ECC_Visibility,
			mCollisionBox->GetCollisionShape(),
			traceParams);

	return hitResults.Num() > 0;
}

bool AGolfBall::lineTrace()
{
	//Linetrace down from actor to find surface location and normal
	if (world && mMesh)
		world->LineTraceMultiByChannel(
			lineTraceResults,
			GetActorLocation(),
			GetActorLocation() + GetActorUpVector() * -400,
			ECollisionChannel::ECC_Pawn,
			traceParams,
			FCollisionResponseParams::DefaultResponseParam);

	//If hit found, create FTransforms
	if (GEngine && lineTraceResults.Num() > 0)
	{
		constructTransform(lineTraceResults[0].ImpactPoint, lineTraceResults[0].ImpactNormal);
	}
	else if (lineTraceResults.Num() == 0)
	{
		constructTransform(GetActorLocation(), FVector(0.f, 0.f, 1.f));
	}

	return lineTraceResults.Num() > 0;
}

void AGolfBall::constructTransform(FVector hitLocation, FVector impactNormal)
{
	surfaceNormal = impactNormal;
	impactPoint = hitLocation;
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + surfaceNormal * 200.f, FColor::Purple, false, 0, (uint8)'\000', 6.f);

	//Variables used to construct new transform
	FVector newForwardVector = FVector::CrossProduct(GetActorRightVector(), surfaceNormal);
	FVector newRightVector = FVector::CrossProduct(surfaceNormal, newForwardVector);

	//FTransform used for actor rotation
	newRotationTransform = FTransform(newForwardVector, newRightVector, surfaceNormal, impactPoint);
}

void AGolfBall::tickWalking(float DeltaTime)
{
	mMesh->SetWorldRotation(currentRotation);

	if (SPressed)
	{ 
		walkingDirection = 180.f;
		movementTransformation(DeltaTime);
	}	
	if (APressed)
	{
		walkingDirection = -90.f;
		movementTransformation(DeltaTime);
	}
	if (DPressed)
	{
		walkingDirection = 90.f;
		movementTransformation(DeltaTime);
	}
	if (WPressed)
	{
		walkingDirection = 0.f;
		movementTransformation(DeltaTime);
	}


	if (WPressed || SPressed || APressed || DPressed)
	{ 
		bIsWalking = true;
		movementTransformation(DeltaTime);
	}

	else
	{ 
		bIsWalking = false;
		mMesh->AddForce(mMesh->GetPhysicsLinearVelocity() * -1 * movementSpeed);
	}

	if (mMesh->GetPhysicsLinearVelocity().Size() > 1500.f)
		mMesh->AddForce(FVector(mMesh->GetPhysicsLinearVelocity().X * -1 * movementSpeed, mMesh->GetPhysicsLinearVelocity().Y * -1 * movementSpeed, mMesh->GetPhysicsLinearVelocity().Z));


	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(4, 1.f, FColor::Yellow, (TEXT("%f"), mMesh->GetPhysicsLinearVelocity().Size()), true);
	
	UE_LOG(LogTemp, Warning, TEXT("%f"), mMesh->GetPhysicsLinearVelocity().Size());

	if (platformJump)
		platformJump = timerFunction(0.2f, DeltaTime);

	if (onGround && hitResults[0].GetActor()->GetHumanReadableName().Compare("movingPlatform") >= 0)
	{
		if(platformOffset.Size() < 2.f && !platformJump)
		{
			platformOffset = GetActorLocation() - hitResults[0].GetActor()->GetActorLocation();
			onPlatform = true;
		}
		if (platformOffset.Size() > 10.f && !platformJump)
			SetActorLocation(hitResults[0].GetActor()->GetActorLocation() + platformOffset);
	}
	else
	{ 
		onPlatform = false;
		platformOffset = FVector::OneVector;
	}

}

void AGolfBall::movementTransformation(float DeltaTime)
{	
	//Reusing variables and snapRotation is camera + inputDirection rotation
	FRotator targetRotation = FRotator(
		newRotationTransform.Rotator().Pitch,
		mController->GetControlRotation().Yaw + walkingDirection,
		newRotationTransform.Rotator().Roll);

	targetRotation.Yaw = targetRotation.Yaw + 90.f;

	FVector newForwardVector = FVector::CrossProduct(targetRotation.Vector(), surfaceNormal);
	FVector newRightVector = FVector::CrossProduct(surfaceNormal, newForwardVector);

	//FTransform used for actor translation
	newTranslationTransform = FTransform(newForwardVector, newRightVector, surfaceNormal, impactPoint);

	mMesh->AddForce(newTranslationTransform.Rotator().Vector() * movementSpeed, NAME_None, true);

	if (onPlatform && onGround)
		platformOffset = GetActorLocation() - hitResults[0].GetActor()->GetActorLocation();
}

void AGolfBall::animationControlTick(float deltaTime)
{
	if (state == FLYING)
	{
		if (bRestartFlyingAnim)
		{
			bRestartFlyingAnim = false;
			bFlyingAnimShouldPlay = true;
			flyingAnimTimer = 0.f;
		}
		if (bFlyingAnimShouldPlay)
		{
			flyingAnimTimer += deltaTime;
			if (flyingAnimTimer >= flyingAnimLength)
			{
				bFlyingAnimShouldPlay = false;
				flyingAnimTimer = 0.f;
			}
		}
	}
}

void AGolfBall::respawnAtCheckpoint()
{
	UGolfSaveGame* LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));
	if (UGameplayStatics::DoesSaveGameExist(LoadGameInstance->slotName, LoadGameInstance->userIndex))
	{
		LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->slotName, LoadGameInstance->userIndex));
		
		int levelIndex = -1;

		for (int i = 0; i < NUM_LEVELS; i++)
		{
			if (LoadGameInstance->levelData[i].levelName.Compare(UGameplayStatics::GetCurrentLevelName(this), ESearchCase::CaseSensitive) == 0)
			{
				levelIndex = i;
			}
		}

		if (levelIndex != -1)
		{
			ACheckpoint* checkpoint = nullptr;
			TArray<AActor*> checkpoints;
			UGameplayStatics::GetAllActorsOfClass(this, ACheckpoint::StaticClass(), checkpoints);

			for (int i = 0; i < checkpoints.Num(); i++)
			{
				if (LoadGameInstance->levelData[levelIndex].currentCheckpoint == Cast<ACheckpoint>(checkpoints[i])->checkpointIndex 
					&& Cast<ACheckpoint>(checkpoints[i])->checkpointIndex != -1)
				{
					checkpoint = Cast<ACheckpoint>(checkpoints[i]);
				}
			}
			if (checkpoint)
			{
				SpawnPosition = checkpoint->GetActorLocation();
				bRespawning = true;
				bStartRespawnCameraFade = true;
			}
			if (!checkpoint)
			{
				TArray<AActor*> PlayerStart;
				UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStart);
				SpawnPosition = PlayerStart[0]->GetActorLocation();
				bRespawning = true;
				bStartRespawnCameraFade = true;
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Invalid level index"));



		//PowerBarWidget->RemoveFromParent();
		//PowerBarWidget_BP->RemoveFromRoot();
	}
}

void AGolfBall::respawnAtCheckpointTick(float deltaTime)
{
	if (bStartRespawnCameraFade)
	{
		GetWorld()->GetFirstPlayerController()->ClientSetCameraFade(true, FColor::Black, FVector2D(0.f, 2.f), cameraFadeTimer);
		bStartRespawnCameraFade = false;
	}
	timeToCameraFadeEnd += deltaTime;
	if (timeToCameraFadeEnd >= cameraFadeTimer)
	{
		mMesh->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f), false);
		SetActorLocation(SpawnPosition + FVector(50.f, 50.f, 300.f));
		GetWorld()->GetFirstPlayerController()->ClientSetCameraFade(true, FColor::Black, FVector2D(1.f, 0.f), cameraFadeTimer / 10);
		bRespawning = false;
		timeToCameraFadeEnd = 0.f;
	}
}

void AGolfBall::confirmLevelSelection()
{
	if (currentLevelSelecter && state == AWAITING_LEVELSELECT_INPUT)
	{
		currentLevelSelecter->LevelSelectWidget->SetVisibility(ESlateVisibility::Hidden);
		//UE_LOG(LogTemp, Warning, TEXT("%s"), *levelToOpen);
		UGameplayStatics::OpenLevel(this, FName(*levelToOpen));
	}
}

void AGolfBall::setLevelToOpen(FString Name)
{
	levelToOpen = Name;
}

void AGolfBall::cameraPanTick()
{

}

void AGolfBall::debugMouse()
{
	debugMouseX = FString::SanitizeFloat(mouseX);
	debugMouseY = FString::SanitizeFloat(572.f - mouseY);
	//if (GEngine)
		//GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("Mouse X: " + debugMouseX + "\n Mouse Y: " + debugMouseY));
}

void AGolfBall::drawDebugObjectsTick()
{
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetForwardVector() * 200, FColor::Red, false, 0, (uint8)'\000', 6.f);
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetUpVector() * 200, FColor::Green, false, 0, (uint8)'\000', 6.f);
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetRightVector() * 200, FColor::Blue, false, 0, (uint8)'\000', 6.f);

}

bool AGolfBall::timerFunction(float timerLength, float DeltaTime)
{
	static float clock = 0.f;
	clock += DeltaTime;

	if (clock > timerLength)
	{
		clock = 0.f;
		return false;
	}
	else
		return true;
}

void AGolfBall::printLoadedGame()
{
	UGolfSaveGame* LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));

	if (UGameplayStatics::DoesSaveGameExist(LoadGameInstance->slotName, LoadGameInstance->userIndex))
	{
		LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->slotName, LoadGameInstance->userIndex));

		for (int i = 0; i < NUM_LEVELS; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Level with index %i and name %s has recorded level data:" 
				"time elapsed(%f), -star rating(%i), current checkpoint(%i), level completed(%b)")
				, 
				i, 
				*LoadGameInstance->levelData[i].levelName, 
				LoadGameInstance->levelData[i].timeElapsed, 
				LoadGameInstance->levelData[i].starRating, 
				LoadGameInstance->levelData[i].currentCheckpoint,
				LoadGameInstance->levelData[i].bLevelCompleted);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save slot not found"));
	}
}

void AGolfBall::setMeshVisibility()
{
	switch (state)
	{
	case GOLF:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh)
		{
			mWingsMeshLeft->SetVisibility(false);
			mWingsMeshRight->SetVisibility(false);
			mLegsMesh->SetVisibility(false);
			//mArmsMesh->SetVisibility(false);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Null pointer at setMeshVisibility()"));
		break;
	case WALKING:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh)
		{
			mWingsMeshLeft->SetVisibility(false);
			mWingsMeshRight->SetVisibility(false);
			mLegsMesh->SetVisibility(true);
			//mArmsMesh->SetVisibility(false);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Null pointer at setMeshVisibility()"));
		break;
	case CLIMBING:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh)
		{
			mWingsMeshLeft->SetVisibility(false);
			mWingsMeshRight->SetVisibility(false);
			mLegsMesh->SetVisibility(false);
			//mArmsMesh->SetVisibility(true);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Null pointer at setMeshVisibility()"));
		break;
	case FLYING:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh)
		{
			mWingsMeshLeft->SetVisibility(true);
			mWingsMeshRight->SetVisibility(true);
			mLegsMesh->SetVisibility(false);
			//mArmsMesh->SetVisibility(false);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Null pointer at setMeshVisibility()"));
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Set mesh visibility failed"));
	}
}