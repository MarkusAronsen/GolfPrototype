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
	mCamera->Activate();

	mSpringArm->bDoCollisionTest = false;

	mWingsMeshLeft->SetupAttachment(mMesh);
	mWingsMeshRight->SetupAttachment(mMesh);
	mLegsMesh->SetupAttachment(mMesh);

	UE_LOG(LogTemp, Warning, TEXT("Golf ball constructed"));
}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();

	UStaticMesh* loadedPlayerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/GBH/Models/Characters/GolfBall_body_asset_static.GolfBall_body_asset_static'"));
	mMesh->SetStaticMesh(loadedPlayerMesh);

	USkeletalMesh* loadedLeftWingMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("SkeletalMesh'/Game/GBH/Models/Characters/WingsSkeletalMesh.WingsSkeletalMesh'"));
	USkeletalMesh* loadedRightWingMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("SkeletalMesh'/Game/GBH/Models/Characters/WingsSkeletalMesh.WingsSkeletalMesh'"));
	mWingsMeshLeft->SetSkeletalMesh(loadedLeftWingMesh);
	mWingsMeshRight->SetSkeletalMesh(loadedRightWingMesh);

	USkeletalMesh* loadedLegsMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("SkeletalMesh'/Game/GBH/Models/Characters/FeetSkeletalMesh.FeetSkeletalMesh'"));
	mLegsMesh->SetSkeletalMesh(loadedLegsMesh);

	mMesh->SetAngularDamping(0.1f);
	mWorldSettings = GetWorldSettings();
	mWorldSettings->bGlobalGravitySet = true;

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

	if (PauseWidget_BP)
	{
		PauseWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PauseWidget_BP);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("PauseWidget not initialized"));

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
		mLegsMesh->SetRelativeLocation(FVector(0, 0, -110));
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
	UAnimBlueprint* flyingAnim = LoadObject<UAnimBlueprint>(nullptr, TEXT("AnimBlueprint'/Game/GBH/Animations/FlyingAnim.FlyingAnim'"));
	UAnimBlueprint* walkAnim = LoadObject<UAnimBlueprint>(nullptr, TEXT("AnimBlueprint'/Game/GBH/Animations/WalkingAnimation.WalkingAnimation'"));

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

	if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel05"), ESearchCase::IgnoreCase) == 0)
	{
		state = WALKING;
	}

	if (mMesh)
	{
		golfInit();
		mMesh->SetEnableGravity(true);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("mMesh not initialized"));

	GEngine->SetMaxFPS(60.f);

	//If level name contains "SecretLevel", retrieve secret level manager from level
	if (UGameplayStatics::GetCurrentLevelName(this).Contains(TEXT("SecretLevel"), ESearchCase::IgnoreCase))
	{
		TArray<AActor*> secretLevelManager;
		UGameplayStatics::GetAllActorsOfClass(this, ASecretLevelManager::StaticClass(), secretLevelManager);
		if (secretLevelManager.Num() > 0)
			secretLevelManagerInstance = Cast<ASecretLevelManager>(secretLevelManager[0]);
		else
			UE_LOG(LogTemp, Warning, TEXT("Secret level manager not found"));

		bPlayingSecretLevel = true;
		UE_LOG(LogTemp, Warning, TEXT("Playing secret level"));

		if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel04") == 0)
			mMesh->SetSimulatePhysics(false);
	}

	//Occlusion outlining
	mMesh->bRenderCustomDepth = true;
	mWingsMeshLeft->bRenderCustomDepth = true;
	mWingsMeshRight->bRenderCustomDepth = true;
	mLegsMesh->bRenderCustomDepth = true;

	mMesh->CustomDepthStencilValue = 1;
	mWingsMeshLeft->CustomDepthStencilValue = 1;
	mWingsMeshRight->CustomDepthStencilValue = 1;
	mLegsMesh->CustomDepthStencilValue = 1;


	UE_LOG(LogTemp, Warning, TEXT("Golf ball initialized"));
}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	onGround = sphereTrace();

	FString velocityString = FString::SanitizeFloat(mMesh->GetPhysicsLinearVelocity().Size());

	switch (state)
	{
	case GOLF:
		//mMesh->AddForce(gravitation * DeltaTime, NAME_None, true);
		lerpPerspective(FRotator(-30.f, 0.f, 0.f), 1000.f, FRotator(10.f, 0.f, 0.f), DeltaTime);
		mouseCameraPitch();
		mouseCameraYaw();
		if (currentLaunchPower > maxLaunchPower)
			currentLaunchPower = maxLaunchPower;

		else if (LMBPressed && canLaunch)
		{
			currentLaunchPower = currentLaunchPower + launchPowerIncrement * DeltaTime;
			if (dirIndicator)
			{
				if (currentLaunchPower <= maxLaunchPower)
				{
					indicatorStretch += DeltaTime;
					dirIndicator->SetActorRelativeScale3D(FVector(1.f + indicatorStretch, 1.f, 1.f));
				}
				dirIndicator->SetActorLocation(GetActorLocation() + FRotator(0.f, world->GetFirstPlayerController()->GetControlRotation().Yaw, 0.f).Vector() * distanceFromBall);
				
				if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel03"))
				{

					//TODO: fix this probably, or just use the debug line idk
					/*
					FVector2D playerScreenLocation;
					world->GetFirstPlayerController()->ProjectWorldLocationToScreen(GetActorLocation(), playerScreenLocation);
					float mouseCoordX, mouseCoordY;
					world->GetFirstPlayerController()->GetMousePosition(mouseCoordX, mouseCoordY);

					dirIndicator->SetActorRotation((FVector(playerScreenLocation.X, playerScreenLocation.Y, 0) - FVector(mouseCoordX, mouseCoordY, 0)).Rotation());*/
				}
				else
					dirIndicator->SetActorRotation(FRotator(0.f, world->GetFirstPlayerController()->GetControlRotation().Yaw, 0.f));

			}
		}

		if (PhysVelPrevFrame < mMesh->GetPhysicsLinearVelocity().Size() && mMesh->GetLinearDamping() > 0.8f)
		{
			mMesh->SetLinearDamping(0.6f);
			mMesh->SetAngularDamping(0.1f);
		}
		if (mMesh->GetPhysicsLinearVelocity().Size() < 700.f && mMesh->GetLinearDamping() < 100.f)
		{
			mMesh->SetLinearDamping(mMesh->GetLinearDamping() + DeltaTime);
			mMesh->SetAngularDamping(mMesh->GetAngularDamping() + DeltaTime);
		}

		PhysVelPrevFrame = mMesh->GetPhysicsLinearVelocity().Size();

		if (mMesh->GetPhysicsLinearVelocity().Size() < 50.f)
			canLaunch = true;
		else
			canLaunch = false;
		break;

	case WALKING:
		lerpPerspective(FRotator(-30.f, 0.f, 0.f), 1000.f, FRotator(10.f, 0.f, 0.f), DeltaTime);
		alignWithSurface = lineTrace();
		mouseCameraPitch();
		mouseCameraYaw();
		tickWalking(DeltaTime);

		currentRotation = FMath::Lerp(
			GetActorRotation(),
			FRotator(
				newRotationTransform.Rotator().Pitch,
				mController->GetControlRotation().Yaw + walkingDirection,
				newRotationTransform.Rotator().Roll),
			lerpTime * DeltaTime);

		if (onGround && mMesh->GetLinearDamping() < 19.f)
			mMesh->SetLinearDamping(20.f);
		if (!onGround && mMesh->GetLinearDamping() > 1.1f)
			mMesh->SetLinearDamping(0.f);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(3, 0.1f, FColor::Yellow, velocityString);
		
		break;

	case CLIMBING:
		world->GetFirstPlayerController()->GetMousePosition(mouseX, mouseY);
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
		lerpPerspective(FRotator(-30, 0.f, 0.f), 1000.f, FRotator(10.f, 0.f, 0.f), DeltaTime);
		mMesh->SetLinearDamping(100.f);
		mouseCameraPitch();
		mouseCameraYaw();
		tickWalking(DeltaTime);
		break;
	};

	//Playing billiards
	if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel03", ESearchCase::IgnoreCase) == 0)
	{
		FVector2D playerScreenLocation;
		world->GetFirstPlayerController()->ProjectWorldLocationToScreen(GetActorLocation(), playerScreenLocation);
		float mouseCoordX, mouseCoordY;
		world->GetFirstPlayerController()->GetMousePosition(mouseCoordX, mouseCoordY);

		billiardsLaunchDirection = FVector(playerScreenLocation.X, playerScreenLocation.Y, 0) - FVector(mouseCoordX, mouseCoordY, 0);
		billiardsLaunchDirection.Normalize();
		billiardsLaunchDirection = billiardsLaunchDirection.RotateAngleAxis(-90, FVector(0, 0, 1));
		//UE_LOG(LogTemp, Warning, TEXT("Player location: %s, mouse location: %s"), *playerScreenLocation.ToString(), *FVector2D(mouseCoordX, mouseCoordY).ToString());

		DrawDebugLine(world, GetActorLocation(), GetActorLocation() + billiardsLaunchDirection * currentLaunchPower * 0.1, FColor::Turquoise, false, 0, (uint8)'\000', 50);

	}


	FVector debugMouseLine = FVector(0.f, mouseX, mouseY) - mousePositionClicked;
	if (debugMouseLine.Size() < 150.f)
		debugMouseLine = FVector::ZeroVector;
	if (debugMouseLine.Size() > 402.f)
	{
		float ratio = debugMouseLine.Size() / 400.f;
		debugMouseLine = debugMouseLine / ratio;
	}
	debugMouseLine = debugMouseLine.RotateAngleAxis(OActorForwardVector.Rotation().Yaw, FVector(0, 0, 1));
	if(LMBPressed && state == CLIMBING)
		DrawDebugLine(world, GetActorLocation(), GetActorLocation() + debugMouseLine, FColor::Blue, false, -1.f, (uint8)'\000', 4.f);
	
	if (world)
		drawDebugObjectsTick();
	debugMouse();

	if(bRespawning)
		respawnAtCheckpointTick(DeltaTime);

	animationControlTick(DeltaTime);
}


// Called to bind functionality to input
void AGolfBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("Spacebar", IE_Pressed, this, &AGolfBall::spacebarPressed);
	InputComponent->BindAction("Spacebar", IE_Released, this, &AGolfBall::spacebarReleased);
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
	InputComponent->BindAction("P", IE_Pressed, this, &AGolfBall::pauseGame);

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
	FVector ballVelocity;
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

	if(state == GOLF && mMesh && mMesh->IsValidLowLevel())
	{ 
		UE_LOG(LogTemp, Warning, TEXT("GOLF INIT"));
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector::ZeroVector;
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 70.f;
		ballVelocity = mMesh->GetPhysicsLinearVelocity();
		mMesh->RecreatePhysicsState();
		mMesh->SetPhysicsLinearVelocity(ballVelocity, false);
		mMesh->SetLinearDamping(0.6f);
		mWorldSettings->GlobalGravityZ = -8000.f;
	}
	if (state == WALKING && mMesh && mMesh->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Warning, TEXT("WALKING INIT"));
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector(0.f, 0.f, -30.f);
		mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 105.f;
		ballVelocity = mMesh->GetPhysicsLinearVelocity();
		mMesh->RecreatePhysicsState();
		mMesh->SetPhysicsLinearVelocity(ballVelocity, false);
		mMesh->SetAngularDamping(0.8f);

		mWorldSettings->GlobalGravityZ = -8000.f;
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
	mMesh->SetLinearDamping(0.6f);
	mWorldSettings->GlobalGravityZ = -8000.f;

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
	mMesh->AddImpulse(FVector(0.f, 0.f, 5500.f), NAME_None, true);
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

	if (state == PLINKO && secretLevelManagerInstance->plinkoLaunchReady)
	{
		secretLevelManagerInstance->startChargingPlinko();
	}
}

void AGolfBall::spacebarReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Spacebar released"));
	if (state == PLINKO)
		secretLevelManagerInstance->plinkoLaunch();
}

void AGolfBall::WClicked()
{
	WPressed = true;
	if (state == PACMAN)
		secretLevelManagerInstance->buffer = FVector(1.f, 0.f, 0.f);
}

void AGolfBall::WReleased()
{
	WPressed = false;
}

void AGolfBall::AClicked()
{
	APressed = true;
	if (state == PACMAN && !secretLevelManagerInstance->gameStarted)
	{
		secretLevelManagerInstance->gameStarted = true;
		SetActorRotation(FVector(0.f, -1.f, 0.f).Rotation());
	}
	if (state == PACMAN && secretLevelManagerInstance->gameStarted)
		secretLevelManagerInstance->buffer = FVector(0.f, -1.f, 0.f);
}

void AGolfBall::AReleased()
{
	APressed = false;
}

void AGolfBall::SClicked()
{
	SPressed = true;
	if (state == PACMAN)
		secretLevelManagerInstance->buffer = FVector(-1.f, 0.f, 0.f);
}

void AGolfBall::SReleased()
{
	SPressed = false;
}

void AGolfBall::DClicked()
{
	DPressed = true;
	if (state == PACMAN && !secretLevelManagerInstance->gameStarted)
	{ 
		secretLevelManagerInstance->gameStarted = true;
		SetActorRotation(FVector(0.f, 1.f, 0.f).Rotation());
	}
	if (state == PACMAN && secretLevelManagerInstance->gameStarted)
		secretLevelManagerInstance->buffer = FVector(0.f, 1.f, 0.f);
}

void AGolfBall::DReleased()
{
	DPressed = false;
}

void AGolfBall::setLMBPressed()
{
	switch (state)
	{
	case GOLF:
		if (canLaunch && PowerBarWidget)
		{
			PowerBarWidget->SetVisibility(ESlateVisibility::Visible);
			LMBPressed = true;
			if (ToSpawn && world && UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel03", ESearchCase::IgnoreCase) != 0)
			{
				spawnInfo.Owner = this;
				spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				dirIndicator = world->SpawnActor<ADirectionIndicator>(ToSpawn, GetActorLocation() + 
					FRotator(0.f, world->GetFirstPlayerController()->GetControlRotation().Yaw, 0.f).Vector() * distanceFromBall, 
					FRotator(0.f, world->GetFirstPlayerController()->GetControlRotation().Yaw, 0.f), spawnInfo);
			}
		}
		break;
	case WALKING:
		LMBPressed = true;
		break;
	case CLIMBING:
		LMBPressed = true;
		if (!mMesh->IsSimulatingPhysics())
			mousePositionClicked = FVector(0.f, mouseX, mouseY);
		break;
	case FLYING:
		LMBPressed = true;
		break;
	}
}
void AGolfBall::setLMBReleased()
{
	LMBPressed = false;
	switch (state)
	{
	case GOLF:
		if (dirIndicator)
		{
			indicatorColor = FVector::ZeroVector;
			indicatorStretch = 0.f;
			dirIndicator->Destroy();
		}
		mMesh->SetLinearDamping(0.6);
		mMesh->SetAngularDamping(0.1);

		if(UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel03", ESearchCase::IgnoreCase) != 0)
			mMesh->AddImpulse(FRotator(0.f, mController->GetControlRotation().Yaw, 0.f).Vector() * currentLaunchPower * 350.f, NAME_None, false);
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ball launched (billiards)"));
			mMesh->AddImpulse(billiardsLaunchDirection * currentLaunchPower * 350.f, NAME_None, false);
			if(currentLaunchPower > 100)
				secretLevelManagerInstance->billiardsShotsUsed++;
		}
		currentLaunchPower = 0.f;
		
		if(PowerBarWidget)
			PowerBarWidget->SetVisibility(ESlateVisibility::Hidden);

		if (bPlayingSecretLevel)
		{
			FString levelName = UGameplayStatics::GetCurrentLevelName(this);
			if (levelName.Compare("SecretLevel01", ESearchCase::IgnoreCase) == 0)
				secretLevelManagerInstance->incrementBowlingThrow();
		}

		break;
	case WALKING:
		break;
	case CLIMBING:
		if (!mMesh->IsSimulatingPhysics())
		{
			mousePositionReleased = FVector(0.f, mouseX, mouseY);
			mousePositionReleased = mousePositionReleased - mousePositionClicked;
			if (mousePositionReleased.Size() < 150.f)
			{ 
				UE_LOG(LogTemp, Warning, TEXT("%f BELOW MINIMUM SIZE"), mousePositionReleased.Size())
				break;
			}
			if (mousePositionReleased.Size() > 400.f)
			{
				float differenceFactor = mousePositionReleased.Size() / 400.f;
				mousePositionReleased = mousePositionReleased / differenceFactor;
				UE_LOG(LogTemp, Warning, TEXT("%f EXCEEDING MAX SIZE"), mousePositionReleased.Size())
			}
			mousePositionReleased = mousePositionReleased.RotateAngleAxis(OActorForwardVector.Rotation().Yaw, FVector(0, 0, 1));
			
			mMesh->SetSimulatePhysics(true);
			mMesh->AddImpulse(mousePositionReleased * 2500.f, NAME_None, false);
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
	//lerpTimer = 0.f;
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
			GetActorLocation() + FVector(0.f, 0.f, -400.f),
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
	if (onGround)
		mMesh->SetWorldRotation(currentRotation);

	bValidInput = true;

	if (APressed && DPressed && SPressed && !WPressed)
		walkingDirection = 180.f;
	else if (APressed && DPressed && WPressed && !SPressed)
		walkingDirection = 0.f;
	else if (SPressed && WPressed && DPressed && !APressed)
		walkingDirection = 90.f;
	else if (SPressed && WPressed && APressed && !DPressed)
		walkingDirection = -90.f;
	else if (WPressed && APressed && !SPressed && !DPressed)
		walkingDirection = -45.f;
	else if (WPressed && DPressed && !APressed && !SPressed)
		walkingDirection = 45.f;
	else if (SPressed && APressed && !WPressed && !DPressed)
		walkingDirection = -135.f;
	else if (SPressed && DPressed && !WPressed && !APressed)
		walkingDirection = 135.f;
	else if (SPressed && !DPressed && !WPressed && !APressed)
		walkingDirection = 180.f;
	else if (APressed && !DPressed && !WPressed && !SPressed)
		walkingDirection = -90.f;
	else if (DPressed && !SPressed && !WPressed && !APressed)
		walkingDirection = 90.f;
	else if (WPressed && !DPressed && !SPressed && !APressed)
		walkingDirection = 0.f;
	else
		bValidInput = false;

	if(bValidInput)
	{ 
		if (WPressed || APressed || SPressed || DPressed)
		{ 
			bIsWalking = true;
			movementTransformation(DeltaTime);
		}
	}
	else
		bIsWalking = false;

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

	FVector newRightVector = FVector::CrossProduct(surfaceNormal, targetRotation.Vector());

	//FTransform used for actor translation
	newTranslationTransform = FTransform(targetRotation.Vector(), newRightVector, surfaceNormal, impactPoint);

	if(onGround)
		mMesh->AddForce(newTranslationTransform.Rotator().Vector() * DeltaTime * movementSpeed, NAME_None, true);

	if(!onGround && mMesh->GetPhysicsLinearVelocity().Size() < 2000.f)
		mMesh->AddForce(newTranslationTransform.Rotator().Vector() * DeltaTime * movementSpeed/8.f, NAME_None, true);

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
		mMesh->SetPhysicsAngularVelocity(FVector(0.f, 0.f, 0.f), false, NAME_None);
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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Yellow, TEXT("Mouse X: " + debugMouseX + "\n Mouse Y: " + debugMouseY));
}

void AGolfBall::drawDebugObjectsTick()
{
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetForwardVector() * 200, FColor::Red, false, 0, (uint8)'\000', 6.f);
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetUpVector() * 200, FColor::Green, false, 0, (uint8)'\000', 6.f);
	DrawDebugLine(GetWorld(), mMesh->GetComponentLocation(), mMesh->GetComponentLocation() + mMesh->GetRightVector() * 200, FColor::Blue, false, 0, (uint8)'\000', 6.f);
}

bool AGolfBall::timerFunction(float timerLength, float DeltaTime)
{
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

void AGolfBall::pauseGame()
{
	UGameplayStatics::SetGamePaused(this, true);
	PauseWidget->AddToViewport();
	PauseWidget->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
}
