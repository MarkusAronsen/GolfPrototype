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
	mArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"), true);

	topDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"), true);
	topDownCamera->SetWorldRotation(FRotator(-90, 0, 0));

	trailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail"));
	canLaunchReadyParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("CanLaunch"));
	transformParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Transform"));


	RootComponent = mMesh;
	mCollisionBox->SetupAttachment(mMesh);
	mSpringArm->SetupAttachment(mMesh);
	mCamera->SetupAttachment(mSpringArm, USpringArmComponent::SocketName);
	mCamera->Activate();

	trailParticles->SetupAttachment(mMesh);
	canLaunchReadyParticles->SetupAttachment(mMesh);
	transformParticles->SetupAttachment(mMesh);

	mWingsMeshLeft->SetupAttachment(mMesh);
	mWingsMeshRight->SetupAttachment(mMesh);
	mLegsMesh->SetupAttachment(mMesh);
	mArmsMesh->SetupAttachment(mMesh);

	UE_LOG(LogTemp, Warning, TEXT("Golf ball constructed"));
}

// Called when the game starts or when spawned
void AGolfBall::BeginPlay()
{
	Super::BeginPlay();

	if(UGameplayStatics::GetCurrentLevelName(this).Compare("LevelSelect", ESearchCase::IgnoreCase) == 0)
		mSpringArm->bDoCollisionTest = true;
	else
		mSpringArm->bDoCollisionTest = false;

	UStaticMesh* loadedPlayerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/GBH/Models/Characters/GolfBall_body_asset_static.GolfBall_body_asset_static'"));
	mMesh->SetStaticMesh(loadedPlayerMesh);

	USkeletalMesh* loadedLeftWingMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("SkeletalMesh'/Game/GBH/Models/Characters/WingsSkeletalMesh.WingsSkeletalMesh'"));
	USkeletalMesh* loadedRightWingMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("SkeletalMesh'/Game/GBH/Models/Characters/WingsSkeletalMesh.WingsSkeletalMesh'"));
	mWingsMeshLeft->SetSkeletalMesh(loadedLeftWingMesh);
	mWingsMeshRight->SetSkeletalMesh(loadedRightWingMesh);

	USkeletalMesh* loadedLegsMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("SkeletalMesh'/Game/GBH/Models/Characters/FeetSkeletalMesh.FeetSkeletalMesh'"));
	mLegsMesh->SetSkeletalMesh(loadedLegsMesh);

	USkeletalMesh* loadedArmsMesh = LoadObject<USkeletalMesh>(nullptr, TEXT("SkeletalMesh'/Game/GBH/Models/Characters/arms_asset.arms_asset'"));
	mArmsMesh->SetSkeletalMesh(loadedArmsMesh);

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
	
	if (SkipCameraPanWidget_BP)
	{
		SkipCameraPanWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), SkipCameraPanWidget_BP);
		
		if (SkipCameraPanWidget)
		{
			SkipCameraPanWidget->AddToViewport();
			SkipCameraPanWidget->SetVisibility(ESlateVisibility::Hidden);	
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("SkipCameraPanWidget not initialized"));
	
	if (FlyingScoreWidget_BP)
	{
		FlyingScoreWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), FlyingScoreWidget_BP);

		if (FlyingScoreWidget)
		{
			FlyingScoreWidget->AddToViewport();
			FlyingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("FlyingScoreWidget not initialized"));

	if (ClimbingScoreWidget_BP)
	{
		ClimbingScoreWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), ClimbingScoreWidget_BP);

		if (ClimbingScoreWidget)
		{
			ClimbingScoreWidget->AddToViewport();
			ClimbingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("ClimbingScoreWidget not initialized"));

	if (WalkingScoreWidget_BP)
	{
		WalkingScoreWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), WalkingScoreWidget_BP);

		if (WalkingScoreWidget)
		{
			WalkingScoreWidget->AddToViewport();
			WalkingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("WalkingScoreWidget not initialized"));

	if (GolfStrokesWidget_BP)
	{
		GolfStrokesWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), GolfStrokesWidget_BP);

		if (GolfStrokesWidget)
		{
			GolfStrokesWidget->AddToViewport();
			GolfStrokesWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("GolfStrokesWidget not initialized"));

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

	if (mLegsMesh && mWingsMeshLeft && mWingsMeshRight && mArmsMesh)
	{
		//Disable visibility on meshes not relevant for golfing
		mLegsMesh->SetVisibility(false);
		mWingsMeshLeft->SetVisibility(false);
		mWingsMeshRight->SetVisibility(false);
		mArmsMesh->SetVisibility(false);
		//-

		//Flip left wing to create right wing
		mWingsMeshRight->SetRelativeScale3D(FVector(1.f, -1.f, 1.f));
		//-

		//Reposition legs
		mLegsMesh->SetRelativeRotation(FRotator(0, -90, 0));
		mLegsMesh->SetRelativeLocation(FVector(0, 0, -110));
		//-

		//Reposition arms
		mArmsMesh->SetRelativeRotation(FRotator(0, -90, 0));
		mArmsMesh->SetRelativeLocation(FVector(0.f, 0.f, -55.f));


	}
	else
		UE_LOG(LogTemp, Warning, TEXT("mLegMesh || mWingsMeshLeft || mWingsMeshRight || mArmsMesh not initialized"));
	
	//Start camera pan if level is not LevelSelect or secret level
	if (UGameplayStatics::GetCurrentLevelName(this).Compare("LevelSelect", ESearchCase::IgnoreCase) != 0
		&& !UGameplayStatics::GetCurrentLevelName(this).Contains("SecretLevel", ESearchCase::IgnoreCase))
	{
		bCameraShouldPan = true;

		UGameplayStatics::GetAllActorsOfClass(this, ACameraActor::StaticClass(), viewTargets);

		if (viewTargets.Num() != 5)
			UE_LOG(LogTemp, Warning, TEXT("viewTargets not 4 (%i)"), viewTargets.Num() + 1);


		UE_LOG(LogTemp, Warning, TEXT("Start camera pan"));
	}

	cameraSpeed = Cast<UGolfGameInstance>(GetGameInstance())->cameraSpeed;

	UClass* flyingAnim = LoadObject<UClass>(nullptr, TEXT("Class'/Game/GBH/Animations/FlyingAnim.FlyingAnim_C'"));
	UClass* walkAnim = LoadObject<UClass>(nullptr, TEXT("Class'/Game/GBH/Animations/WalkingAnimation.WalkingAnimation_C'"));
	UClass* climbingAnim = LoadObject<UClass>(nullptr, TEXT("Class'/Game/GBH/Animations/ClimbingAnimation.ClimbingAnimation_C'"));


	if (flyingAnim)
	{
		mWingsMeshLeft->SetAnimInstanceClass(flyingAnim);
		mWingsMeshRight->SetAnimInstanceClass(flyingAnim);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find flying animation"));

	if (walkAnim)
		mLegsMesh->SetAnimInstanceClass(walkAnim);
	else
	UE_LOG(LogTemp, Warning, TEXT("Could not find walking animation"));

	if (climbingAnim)
		mArmsMesh->SetAnimInstanceClass(climbingAnim);
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find the climbing animation"));
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
	
	if(GEngine)
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

	//Shadows off, custom decal is used
	mMesh->CastShadow = false;
	mLegsMesh->CastShadow = false;
	mWingsMeshLeft->CastShadow = false;
	mWingsMeshRight->CastShadow = false;

	//Fetch particle systems
	UParticleSystem* LoadTrailParticles = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/GBH/Particles/TestParticles/TrailParticles.TrailParticles'"));

	if (LoadTrailParticles)
		trailParticles->SetTemplate(LoadTrailParticles);

	else
		UE_LOG(LogTemp, Warning, TEXT("Trail particles not found"));

	UParticleSystem* LoadCanLaunchParticles = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/GBH/Particles/TestParticles/GreenSparksNoLoop.GreenSparksNoLoop'"));

	if (LoadCanLaunchParticles)
		canLaunchReadyParticles->SetTemplate(LoadCanLaunchParticles);
	else
		UE_LOG(LogTemp, Warning, TEXT("Can launch particles not found"));

	UParticleSystem* LoadTransformParticles = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/GBH/Particles/Particles/Cloud_Poof.Cloud_Poof'"));

        //Trail is work in progress
        trailParticles->Deactivate();

	if (LoadTransformParticles)
		transformParticles->SetTemplate(LoadTransformParticles);
	else
		UE_LOG(LogTemp, Warning, TEXT("Transform particles not found"));

	UE_LOG(LogTemp, Warning, TEXT("Golf ball initialized"));
}

// Called every frame
void AGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	onGround = sphereTrace();

	FString sizeString = FString::SanitizeFloat(mMesh->GetPhysicsLinearVelocity().Size());
	FString linearDampingString = FString::SanitizeFloat(mMesh->GetLinearDamping());
	FString velocityString = FString::SanitizeFloat(mMesh->GetPhysicsLinearVelocity().Size());

	FString angularVelocityString = mMesh->GetPhysicsAngularVelocity().ToString();
	FString stringStretch;

	switch (state)
	{
	case GOLF:
		if(bLerpingPerspective)
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
				dirIndicator->SetActorRotation(FRotator(0.f, world->GetFirstPlayerController()->GetControlRotation().Yaw, 0.f));

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
			}
		}

		if (PhysVelPrevFrame < mMesh->GetPhysicsLinearVelocity().Size() && mMesh->GetLinearDamping() > 0.8f)
		{
			mMesh->SetLinearDamping(0.6f);
			mMesh->SetAngularDamping(0.1f);
		}
		if (mMesh->GetPhysicsLinearVelocity().Size() < 700.f && mMesh->GetLinearDamping() < 100.f)
		{
			mMesh->SetLinearDamping(mMesh->GetLinearDamping() + DeltaTime * 3);
			mMesh->SetAngularDamping(mMesh->GetAngularDamping() + DeltaTime * 3);
		}

		PhysVelPrevFrame = mMesh->GetPhysicsLinearVelocity().Size();

		if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel01"), ESearchCase::IgnoreCase) != 0)
		{
			if (mMesh->GetPhysicsLinearVelocity().Size() < 50.f)
			{
				canLaunch = true;
				if (!canLaunchParticlesHaveActivated)
				{
					//canLaunchReadyParticles->Activate();
					canLaunchParticlesHaveActivated = true;
				}
			}
			else
			{
				canLaunch = false;
				canLaunchParticlesHaveActivated = false;
			}
		}
		else
			canLaunch = false;

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(16, 0.1f, FColor::Yellow, *sizeString);
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(17, 0.1f, FColor::Yellow, *linearDampingString);
		break;

	case WALKING:
		if(bLerpingPerspective)
			lerpPerspective(FRotator(-30.f, 0.f, 0.f), 1000.f, FRotator(10.f, 0.f, 0.f), DeltaTime);

		lineTraceHit = lineTrace();
		mouseCameraPitch();
		mouseCameraYaw();
		tickWalking(DeltaTime);

		if (onGround && mMesh->GetLinearDamping() < 19.f)// && !onMoveablePlatform)
			mMesh->SetLinearDamping(20.f);
		if (!onGround && mMesh->GetLinearDamping() > 1.1f)// && !onMoveablePlatform)
			mMesh->SetLinearDamping(0.1f);

		currentRotation = FMath::Lerp(
			GetActorRotation(),
			FRotator(
				newRotationTransform.Rotator().Pitch,
				walkingDirection,
				newRotationTransform.Rotator().Roll),
			15.f * DeltaTime);
		

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(17, 0.1f, FColor::Purple, *linearDampingString);


		break;

	case CLIMBING:
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		world->GetFirstPlayerController()->GetMousePosition(mouseX, mouseY);
		if(bLerpingPerspective)
			lerpPerspective(GetActorRotation(), 2500.f, FRotator(0.f, 0.f, 0.f), DeltaTime);
		debugMouseLine = FVector(0.f, mouseX, mouseY) - mousePositionClicked;
		debugMouseLine = debugMouseLine * 2.f;
		if (debugMouseLine.Size() < 100.f)
			debugMouseLine = FVector::ZeroVector;
		if (debugMouseLine.Size() > 402.f)
		{
			ratio = debugMouseLine.Size() / 400.f;
			debugMouseLine = debugMouseLine / ratio;
		}
		debugMouseLine = debugMouseLine.RotateAngleAxis(OActorForwardVector.Rotation().Yaw, FVector(0, 0, 1));

		if (mousePositionClicked.Size() > 1.f)
		{
			bLerpingPerspective = false;

			FVector A = FVector(0, 255, 0);
			FVector B = FVector(255, 155, 155);

			FVector lineColorVector = A + FVector((debugMouseLine.Size() / 400.f), (debugMouseLine.Size() / 400.f), 0) * (B - A);

			FColor lineColor = FColor(lineColorVector.X, lineColorVector.Y, lineColorVector.Z);

			stretchRatio = debugMouseLine.Size();

			//DrawDebugLine(world, GetActorLocation(), GetActorLocation() + debugMouseLine, lineColor, false, -1.f, (uint8)'\000', 10.f);
			
			FVector climbRotation;
			climbRotation = (GetActorLocation() - (currentClimbObject->GetActorLocation() + currentClimbObject->GetActorForwardVector() * 50));
			climbRotation.Normalize();
			FVector downVector = FVector(0, 0, -1);
			float dotProduct = FVector::DotProduct(climbRotation, downVector);
			float climbingDegree = (acos(dotProduct) * 180)/PI;
			FString degreeString = FString::SanitizeFloat(dotProduct);

			GEngine->AddOnScreenDebugMessage(11, 0.1f, FColor::Red, *mCamera->GetComponentRotation().ToString());

			if (mousePositionClicked.Y <= mouseX)
				climbingDegree = climbingDegree * -1;
			
			if (!currentClimbObject->bIsEdgeNode && debugMouseLine.Size() > 100.f)
				SetActorRotation(FRotator(0.f, currentClimbObject->GetActorRotation().Yaw + 180.f, climbingDegree));

			if (!currentClimbObject->bIsEdgeNode && debugMouseLine.Size() <= 100.f)
				SetActorRotation(FRotator(0.f, currentClimbObject->GetActorRotation().Yaw + 180.f, 0.f));

			if (currentClimbObject->bIsEdgeNode)
			{
				if(mousePositionClicked.Y <= mouseX && debugMouseLine.Size() > 100.f)
					SetActorRotation(FRotator(0.f, currentClimbObject->GetActorRotation().Yaw + 180.f + 45.f, climbingDegree));
				if (mousePositionClicked.Y > mouseX && debugMouseLine.Size() > 100.f)
					SetActorRotation(FRotator(0.f, currentClimbObject->GetActorRotation().Yaw + 180.f - 45.f, climbingDegree));
			}

			if(currentClimbObject && !mMesh->IsSimulatingPhysics())
				SetActorLocation((currentClimbObject->GetActorLocation() + OActorForwardVector * 50) + debugMouseLine * -1 * 0.3);
		}
		
		stringStretch = FString::SanitizeFloat(stretchRatio);
		
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(5, 0.1f, FColor::Purple, *stringStretch);
		break;

	case FLYING:
		if (!flyingGravityFlipped)
		{
			lerpPerspective(GetActorRightVector().Rotation() + FRotator(0.f, 180.f, 0.f), 2000.f, FRotator(0.f, 0.f, 0.f), DeltaTime * 2.5f);
			SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, mCamera->GetComponentRotation().Roll));
		}

		else
		{
			lerpPerspective(GetActorRightVector().Rotation() + FRotator(0.f, 180.f, 0.f), 2000.f, FRotator(0.f, 0.f, 180.f), DeltaTime * 2.5f);
			SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, mCamera->GetComponentRotation().Roll));
		}

		if (easeGravityShift)
		{
			easeTimer += DeltaTime;

			gravity = FVector::ZeroVector;

			if (easeTimer >= 0.75f)
			{
				easeTimer = 0.f;
				easeGravityShift = false;
			}
		}
		else
			gravity = FVector(0, 0, gravityZ);

		applyForce(gravity);

		/*if (flyingGravityFlipped)
		{
			if (gravityZ <= 1.5f)
			{
				gravityZ += DeltaTime * 15;
				velocity = velocity * 0.1;
			}
		}
		else
		{
			if (gravityZ >= -1.5f)
			{
				gravityZ -= DeltaTime * 15;
				velocity = velocity * 0.1;
			}

		}*/
		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (gravity * 200), FColor::Blue, false, -1.0f, (uint8)'\000', 20);

		updatePosition(DeltaTime);
		break;

	case LEVEL_SELECT:
		topDownCamera->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 2500));
		break;

	case AWAITING_LEVELSELECT_INPUT:
		if(bLerpingPerspective)
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

	if (world)
		drawDebugObjectsTick();
	

	//
	//debugMouse();

	if(bRespawning)
		respawnAtCheckpointTick(DeltaTime);

	animationControlTick(DeltaTime);

	if (bCameraShouldPan)
		cameraPanTick(DeltaTime);
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
	InputComponent->BindAction("Enter", IE_Pressed, this, &AGolfBall::enterPressed);

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
		currentClimbObject = Cast<AClimbObject>(OtherActor);
		bLerpingPerspective = true;
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
	transformParticles->Activate();

	FVector ballVelocity;

	bLerpingPerspective = true;
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
		mWorldSettings->GlobalGravityZ = -8000.f;

		if (strokeCounter > 0)
		{
			GolfStrokesWidget->SetVisibility(ESlateVisibility::Visible);
			WalkingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
			FlyingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
			ClimbingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		}
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

		if (walkingRestarts > 0)
		{
			GolfStrokesWidget->SetVisibility(ESlateVisibility::Hidden);
			WalkingScoreWidget->SetVisibility(ESlateVisibility::Visible);
			FlyingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
			ClimbingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	setMeshVisibility();
	switchDecalVisibility(true);
}

void AGolfBall::climbingInit(AActor* OtherActor)
{
	transformParticles->Activate();

	state = CLIMBING;
	
	bClimbInAir = false;
	bLerpingPerspective = true;
	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector::ZeroVector;
	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 70.f;
	mMesh->RecreatePhysicsState();

	mMesh->SetSimulatePhysics(false);
	mMesh->SetGenerateOverlapEvents(true);
	world->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	if(!currentClimbObject->bIsEdgeNode)
		SetActorLocation(OtherActor->GetActorLocation() + OtherActor->GetActorForwardVector() * 50.f);
	if (currentClimbObject->bIsEdgeNode)
		SetActorLocation(OtherActor->GetActorLocation() + OtherActor->GetActorForwardVector() * 100.f);
	OActorForwardVector = OtherActor->GetActorForwardVector();
	SetActorRotation(FRotator(0.f, OtherActor->GetActorRotation().Yaw + 180.f, 0.f));
	mMesh->SetLinearDamping(0.6f);
	mWorldSettings->GlobalGravityZ = -8000.f;

	mSpringArm->bInheritYaw = false;
	mSpringArm->CameraLagSpeed = 5.f;

	setMeshVisibility();
	switchDecalVisibility(false);

	if (climbingRestarts > 0)
	{
		GolfStrokesWidget->SetVisibility(ESlateVisibility::Hidden);
		WalkingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		FlyingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		ClimbingScoreWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AGolfBall::flyingInit(AActor *OtherActor)
{
	transformParticles->Activate();

	state = FLYING;

	bLerpingPerspective = true;
	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Center = FVector::ZeroVector;
	mMesh->GetStaticMesh()->BodySetup->AggGeom.SphereElems[0].Radius = 70.f;
	mMesh->RecreatePhysicsState();

	velocity = FVector::ZeroVector;

	mMesh->SetSimulatePhysics(false);
	SetActorLocation(OtherActor->GetActorLocation());
	SetActorRotation(OtherActor->GetActorRotation());

	position = OtherActor->GetActorLocation();

	mSpringArm->bInheritYaw = false;

	setMeshVisibility();
	switchDecalVisibility(false);

	if (flyingRestarts > 0)
	{
		GolfStrokesWidget->SetVisibility(ESlateVisibility::Visible);
		WalkingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		FlyingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
		ClimbingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
	}

}

void AGolfBall::lerpPerspective(FRotator springToRot, float springToLength, FRotator camToRot, float DeltaTime)
{
	if (!springToRot.Equals(mSpringArm->RelativeRotation, 0.001f))
		mSpringArm->RelativeRotation = FMath::RInterpTo(mSpringArm->RelativeRotation, springToRot, DeltaTime, 3.f);
	if (!FMath::IsNearlyEqual(springToLength, mSpringArm->TargetArmLength, 0.001f))
		mSpringArm->TargetArmLength = FMath::FInterpTo(mSpringArm->TargetArmLength, springToLength, DeltaTime, 3.f);
	if (!camToRot.Equals(mCamera->RelativeRotation, 0.001f))
		mCamera->SetRelativeRotation(FMath::RInterpTo(mCamera->RelativeRotation, camToRot, DeltaTime, 3.f));
	if (camToRot.Equals(mCamera->RelativeRotation, 0.5f) && FMath::IsNearlyEqual(springToLength, mSpringArm->TargetArmLength, 2.f) && springToRot.Equals(mSpringArm->RelativeRotation, 0.5f))
		bLerpingPerspective = false;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(12, 0.1f, FColor::Blue, TEXT("LERPING PERSPECTIVE!"));
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
	if(WPressed || APressed || SPressed || DPressed)
		mMesh->SetPhysicsAngularVelocityInDegrees(GetActorRightVector() * 800, false, NAME_None);
	if (onPlatform)
		platformJump = true;
}

void AGolfBall::applyForce(FVector force)
{
	acceleration += force;
}

void AGolfBall::updatePosition(float DeltaTime)
{
	if (!easeGravityShift)
	{
		velocity += acceleration;

		position += velocity;
		position += FVector(GetActorForwardVector() * 700 * DeltaTime);
		acceleration = FVector::ZeroVector;

		SetActorLocation(position);
	}
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
	if (!bCameraShouldPan)
	{

	
	if (state == FLYING)
	{
		velocity = FVector::ZeroVector;

		if (!easeGravityShift)
		{
			//if (!flyingGravityFlipped)
			applyForce(FVector(0, 0, 30));
			//else
				//applyForce(FVector(0, 0, -30));

			bRestartFlyingAnim = true;
		}
		else
			bRestartFlyingAnim = true;

		
	}
	if (state == WALKING && lineTraceHit)
		jump();

	if (state == PLINKO && secretLevelManagerInstance->plinkoLaunchReady)
	{
		secretLevelManagerInstance->startChargingPlinko();
	}
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
	if (!bCameraShouldPan)
	{
		WPressed = true;
		if (state == PACMAN)
			secretLevelManagerInstance->buffer = FVector(1.f, 0.f, 0.f);

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateW = true;
		}
	}
}

void AGolfBall::WReleased()
{
	if (!bCameraShouldPan)
	{
		WPressed = false;

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateW = false;
		}
	}
}

void AGolfBall::AClicked()
{
	if (!bCameraShouldPan)
	{
		APressed = true;
		if (state == PACMAN && !secretLevelManagerInstance->gameStarted)
		{
			secretLevelManagerInstance->gameStarted = true;
			SetActorRotation(FVector(0.f, -1.f, 0.f).Rotation());
		}
		if (state == PACMAN && secretLevelManagerInstance->gameStarted)
			secretLevelManagerInstance->buffer = FVector(0.f, -1.f, 0.f);

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateA = true;
		}
	}
}

void AGolfBall::AReleased()
{
	if (!bCameraShouldPan)
	{
		APressed = false;

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateA = false;
		}
	}
}

void AGolfBall::SClicked()
{
	if (!bCameraShouldPan)
	{
		SPressed = true;
		if (state == PACMAN)
			secretLevelManagerInstance->buffer = FVector(-1.f, 0.f, 0.f);

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateS = true;
		}
	}
}

void AGolfBall::SReleased()
{
	if (!bCameraShouldPan)
	{


		SPressed = false;

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateS = false;
		}
	}
}

void AGolfBall::DClicked()
{
	if (!bCameraShouldPan)
	{
		DPressed = true;
		if (state == PACMAN && !secretLevelManagerInstance->gameStarted)
		{
			secretLevelManagerInstance->gameStarted = true;
			SetActorRotation(FVector(0.f, 1.f, 0.f).Rotation());
		}
		if (state == PACMAN && secretLevelManagerInstance->gameStarted)
			secretLevelManagerInstance->buffer = FVector(0.f, 1.f, 0.f);

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateD = true;
		}
	}
}

void AGolfBall::DReleased()
{
	if (!bCameraShouldPan)
	{
		DPressed = false;

		if (secretLevelManagerInstance && secretLevelManagerInstance->secretState == SecretLevelState::MAZE)
		{
			secretLevelManagerInstance->mazeRotateD = false;
		}
	}
}

void AGolfBall::setLMBPressed()
{
	if (!bCameraShouldPan)
	{
		switch (state)
		{
		case GOLF:
			if (canLaunch && PowerBarWidget && !bRespawning)
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
			UE_LOG(LogTemp, Warning, TEXT("LMBPressed!"));
			if (!mMesh->IsSimulatingPhysics())
				mousePositionClicked = FVector(0.f, mouseX, mouseY);
			break;
		case FLYING:
			LMBPressed = true;
			break;
		}
	}
}

void AGolfBall::setLMBReleased()
{
	if (!bCameraShouldPan)
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

			if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel03", ESearchCase::IgnoreCase) != 0)
			{

				if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel01", ESearchCase::IgnoreCase) == 0 && !secretLevelManagerInstance->bBallIsThrown)
				{
					if (canLaunch)
					{
						secretLevelManagerInstance->incrementBowlingThrow();
						mMesh->AddImpulse(FRotator(0.f, mController->GetControlRotation().Yaw, 0.f).Vector() * currentLaunchPower * 350.f, NAME_None, false);
						canLaunch = false;
					}
				}
				/*else if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel01", ESearchCase::IgnoreCase) == 0 && secretLevelManagerInstance->bBallIsThrown)
				{

				}*/
				else
					mMesh->AddImpulse(FRotator(0.f, mController->GetControlRotation().Yaw, 0.f).Vector() * currentLaunchPower * 350.f, NAME_None, false);

			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Ball launched (billiards)"));
				mMesh->AddImpulse(billiardsLaunchDirection * currentLaunchPower * 350.f, NAME_None, false);
				if (currentLaunchPower > 100)
					secretLevelManagerInstance->billiardsShotsUsed++;
			}
			currentLaunchPower = 0.f;

			if (PowerBarWidget)
				PowerBarWidget->SetVisibility(ESlateVisibility::Hidden);

			break;
		case WALKING:
			break;
		case CLIMBING:
			shouldLaunch = true;
			if (!mMesh->IsSimulatingPhysics())
			{
				mousePositionReleased = FVector(0.f, mouseX, mouseY);
				mousePositionReleased = mousePositionReleased - mousePositionClicked;
				mousePositionReleased = mousePositionReleased * 2.f;
				if (mousePositionReleased.Size() < 100.f)
				{
					//UE_LOG(LogTemp, Warning, TEXT("%f BELOW MINIMUM SIZE"), mousePositionReleased.Size())
					shouldLaunch = false;
					debugMouseLine = FVector::ZeroVector;
					mousePositionClicked = FVector::ZeroVector;
					mousePositionReleased = FVector::ZeroVector;
				}
				if (mousePositionReleased.Size() > 400.f)
				{
					float differenceFactor = mousePositionReleased.Size() / 400.f;
					mousePositionReleased = mousePositionReleased / differenceFactor;
					//UE_LOG(LogTemp, Warning, TEXT("%f EXCEEDING MAX SIZE"), mousePositionReleased.Size())
				}
				if (shouldLaunch)
				{
					if (!currentClimbObject->bIsEdgeNode)
						mousePositionReleased = mousePositionReleased.RotateAngleAxis(OActorForwardVector.Rotation().Yaw, FVector(0, 0, 1));
					if (currentClimbObject->bIsEdgeNode && mousePositionClicked.Y >= mouseX)
						mousePositionReleased = mousePositionReleased.RotateAngleAxis(OActorForwardVector.Rotation().Yaw - 45, FVector(0, 0, 1));
					if (currentClimbObject->bIsEdgeNode && mousePositionClicked.Y < mouseX)
						mousePositionReleased = mousePositionReleased.RotateAngleAxis(OActorForwardVector.Rotation().Yaw + 45, FVector(0, 0, 1));

					mMesh->SetSimulatePhysics(true);
					mMesh->AddImpulse(mousePositionReleased * 2500.f, NAME_None, false);

					stretchRatio = 0.f;
					debugMouseLine = FVector::ZeroVector;
					mousePositionClicked = FVector::ZeroVector;
					mousePositionReleased = FVector::ZeroVector;
					bLerpingPerspective = true;
					bClimbInAir = true;
					//Cast<AClimbObject>(currentClimbObject)->ignored = true;

				}
				break;
		case FLYING:
			break;
			}
		}
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

void AGolfBall::enterPressed()
{
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, 2, EViewTargetBlendFunction::VTBlend_Cubic, 0, true);
	bCameraShouldPan = false;
	mCamera->Activate();
	SkipCameraPanWidget->RemoveFromParent();
}

void AGolfBall::scrollUp()
{
	if (mSpringArm->TargetArmLength > 500.f && !bLerpingPerspective)
		mSpringArm->TargetArmLength -= 100.f;
}

void AGolfBall::scrollDown()
{
	if (mSpringArm->TargetArmLength < 2000.f && !bLerpingPerspective)
		mSpringArm->TargetArmLength += 100.f;
}

bool AGolfBall::sphereTrace()
{
	if (world && mMesh)
		world->SweepMultiByChannel(
			hitResults,
			mMesh->GetComponentToWorld().GetLocation(),
			mMesh->GetComponentToWorld().GetLocation() + (FVector(0.f, 0.f, 1.f) * -120.f),
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
	if (lineTraceHit)
		mMesh->SetWorldRotation(currentRotation);

	bValidInput = true;

	if (APressed && DPressed && SPressed && !WPressed)
		walkingDirection = mController->GetControlRotation().Yaw + 180.f;
	else if (APressed && DPressed && WPressed && !SPressed)
		walkingDirection = mController->GetControlRotation().Yaw + 0.f;
	else if (SPressed && WPressed && DPressed && !APressed)
		walkingDirection = mController->GetControlRotation().Yaw + 90.f;
	else if (SPressed && WPressed && APressed && !DPressed)
		walkingDirection = mController->GetControlRotation().Yaw - 90.f;
	else if (WPressed && APressed && !SPressed && !DPressed)
		walkingDirection = mController->GetControlRotation().Yaw - 45.f;
	else if (WPressed && DPressed && !APressed && !SPressed)
		walkingDirection = mController->GetControlRotation().Yaw + 45.f;
	else if (SPressed && APressed && !WPressed && !DPressed)
		walkingDirection = mController->GetControlRotation().Yaw - 135.f;
	else if (SPressed && DPressed && !WPressed && !APressed)
		walkingDirection = mController->GetControlRotation().Yaw + 135.f;
	else if (SPressed && !DPressed && !WPressed && !APressed)
		walkingDirection = mController->GetControlRotation().Yaw + 180.f;
	else if (APressed && !DPressed && !WPressed && !SPressed)
		walkingDirection = mController->GetControlRotation().Yaw - 90.f;
	else if (DPressed && !SPressed && !WPressed && !APressed)
		walkingDirection = mController->GetControlRotation().Yaw + 90.f;
	else if (WPressed && !DPressed && !SPressed && !APressed)
		walkingDirection = mController->GetControlRotation().Yaw + 0.f;
	else
		bValidInput = false;

	if(bValidInput)
	{ 
		if (WPressed || APressed || SPressed || DPressed)
		{ 
			movementTransformation(DeltaTime);
		}
	}

	if (platformJump)
		platformJump = timerFunction(0.2f, DeltaTime);

	if (onGround)

	{
		if (hitResults[0].GetActor()->GetHumanReadableName().Compare("movingPlatform") >= 0 || hitResults[0].GetActor()->GetHumanReadableName().Compare("MoveablePlatform") >= 0)
		{
			if (platformOffset.Size() < 2.f && !platformJump)
			{
				platformOffset = GetActorLocation() - hitResults[0].GetActor()->GetActorLocation();
				onPlatform = true;
			}
			if (platformOffset.Size() > 10.f && !platformJump)
				SetActorLocation(hitResults[0].GetActor()->GetActorLocation() + platformOffset);
		}
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
		walkingDirection,
		newRotationTransform.Rotator().Roll);

	FVector newRightVector = FVector::CrossProduct(surfaceNormal, targetRotation.Vector());

	//FTransform used for actor translation
	newTranslationTransform = FTransform(targetRotation.Vector(), newRightVector, surfaceNormal, impactPoint);

	if(onGround)
		mMesh->AddForce((newTranslationTransform.Rotator().Vector() * movementSpeed) * DeltaTime, NAME_None, true);

	FVector XYLength = FVector(mMesh->GetPhysicsLinearVelocity().X, mMesh->GetPhysicsLinearVelocity().Y, 0.f);
	FVector XYNormalized = XYLength;
	XYNormalized.Normalize(0.01f);
	FVector inputVelocityNormalized = newTranslationTransform.Rotator().Vector();
	float directionDistance = FMath::Clamp((inputVelocityNormalized - XYNormalized).Size() * 0.1f, 0.f, 0.1f);
	FString distanceString = FString::SanitizeFloat(directionDistance);

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(17, 0.1f, FColor::Yellow, *distanceString);

	if (!onGround && XYLength.Size() < 1000.f)
		mMesh->AddForce((newTranslationTransform.Rotator().Vector() * movementSpeed * 100.f) * DeltaTime, NAME_None, false);

	if (!onGround && XYLength.Size() > 1000.f)
		mMesh->SetPhysicsLinearVelocity(FVector(mMesh->GetPhysicsLinearVelocity().X * 0.98f, mMesh->GetPhysicsLinearVelocity().Y * 0.98f, mMesh->GetPhysicsLinearVelocity().Z), false, NAME_None);

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
		SetActorLocation(SpawnPosition + FVector(50.f, 50.f, 300.f));
		mMesh->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 0.f), false);
		mMesh->SetPhysicsAngularVelocity(FVector(0.f, 0.f, 0.f), false, NAME_None);
					
		GetWorld()->GetFirstPlayerController()->ClientSetCameraFade(true, FColor::Black, FVector2D(1.f, 0.f), cameraFadeTimer / 10);

		bRespawning = false;
		timeToCameraFadeEnd = 0.f;

		if (flyingGravityFlipped)
			flyingGravityFlipped = false;

		if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel01", ESearchCase::IgnoreCase) == 0)
		{
			secretLevelManagerInstance->bBallIsThrown = false;
			canLaunch = true;
		}
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

void AGolfBall::cameraPanTick(float deltaTime)
{
	//Set initial view target
	if (currentViewTarget == 0)
	{
		for (int i = 0; i < viewTargets.Num(); i++)
		{
			if (viewTargets[i]->ActorHasTag("Target0"))
				target = viewTargets[i];
		}
		if (target)
			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(target, viewTargetBlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 3, true);
		else
			UE_LOG(LogTemp, Warning, TEXT("view target with tag: Target0 not found"));

		currentViewTarget++;

		SkipCameraPanWidget->SetVisibility(ESlateVisibility::Visible);
	}

	blendTimer += deltaTime;

	if (blendTimer >= viewTargetBlendTime)
	{
		if (currentViewTarget == 5)
		{
			bCameraShouldPan = false;
			return;
		}

		switch (currentViewTarget)
		{
		case 1:
			for (int i = 0; i < viewTargets.Num(); i++)
			{
				if (viewTargets[i]->ActorHasTag("Target1"))
					target = viewTargets[i];
			}
			if (target)
				GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(target, viewTargetBlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 3, true);
			else
				UE_LOG(LogTemp, Warning, TEXT("view target with tag: Target1 not found"));
			break;

		case 2:
			for (int i = 0; i < viewTargets.Num(); i++)
			{
				if (viewTargets[i]->ActorHasTag("Target2"))
					target = viewTargets[i];
			}
			if (target)
				GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(target, viewTargetBlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 3, true);
			else
				UE_LOG(LogTemp, Warning, TEXT("view target with tag: Target2 not found"));
			break;

		case 3:
			for (int i = 0; i < viewTargets.Num(); i++)
			{
				if (viewTargets[i]->ActorHasTag("Target3"))
					target = viewTargets[i];
			}
			if (target)
				GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(target, viewTargetBlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 3, true);
			else
				UE_LOG(LogTemp, Warning, TEXT("view target with tag: Target3 not found"));
			break;

		case 4:
			GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, viewTargetBlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 3, true);
			mCamera->Activate();
			UE_LOG(LogTemp, Warning, TEXT("Setting view target to player"));
			SkipCameraPanWidget->RemoveFromParent();
			break;
		}
		blendTimer = 0.f;
		if (currentViewTarget < 5)
		{
			currentViewTarget++;
		}
	}
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

	TArray<FString> dialogue;

	dialogue.Empty();

	dialogue.Add("The Saint Paul River is a river of western Africa. Its headwaters are in southeastern Guinea. Its upper portion in Guinea is known as the Diani River or Niandi River, and forms part of the boundary between Guinea and Liberia.");

	dialogue.Add("The river then enters Liberia about 50 km(31 mi) north of Gbarnga and crosses Liberia in a southwesterly direction.It empties into the Atlantic Ocean at Cape Mesurado in Monrovia near Bushrod Island, separating Monrovia from its suburb Brewerville.");

	dialogue.Add("Line 3");
	
	printDialogue(dialogue);

}

void AGolfBall::setMeshVisibility()
{
	switch (state)
	{
	case GOLF:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh && mArmsMesh)
		{
			mWingsMeshLeft->SetVisibility(false);
			mWingsMeshRight->SetVisibility(false);
			mLegsMesh->SetVisibility(false);
			mArmsMesh->SetVisibility(false);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Null pointer at setMeshVisibility()"));
		break;
	case WALKING:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh && mArmsMesh)
		{
			mWingsMeshLeft->SetVisibility(false);
			mWingsMeshRight->SetVisibility(false);
			mLegsMesh->SetVisibility(true);
			mArmsMesh->SetVisibility(false);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Null pointer at setMeshVisibility()"));
		break;
	case CLIMBING:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh && mArmsMesh)
		{
			mWingsMeshLeft->SetVisibility(false);
			mWingsMeshRight->SetVisibility(false);
			mLegsMesh->SetVisibility(false);
			mArmsMesh->SetVisibility(true);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Null pointer at setMeshVisibility()"));
		break;
	case FLYING:
		if (mWingsMeshLeft && mWingsMeshRight && mLegsMesh && mArmsMesh)
		{
			mWingsMeshLeft->SetVisibility(true);
			mWingsMeshRight->SetVisibility(true);
			mLegsMesh->SetVisibility(false);
			mArmsMesh->SetVisibility(false);
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
