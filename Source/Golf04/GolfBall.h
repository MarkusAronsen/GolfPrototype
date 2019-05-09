// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DrawDebugHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/GameEngine.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GolfBall.generated.h"

class ALevelSelecter;
class AClimbObject;
class AGoal;
class AGolfSaveGame;
class ACheckpoint;
class AGolfGameInstance;
class ATransformationObject;
class ASecretLevelManager;
class ADirectionIndicator;
class ADestructableBlock;
class AFlyingGravitySwitch;
class AClimbRisingFloor;
class APlinkoBlocker;

UCLASS()
class GOLF04_API AGolfBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGolfBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UWorld* world;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* mCamera = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* mSpringArm = nullptr;

	//UPROPERTY(Category = "Component")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* mCollisionBox = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mVisibleMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mWingsMeshLeft = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mWingsMeshRight = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mLegsMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mArmsMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mPacManMesh = nullptr;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimationAsset* FlyingAnim;*/

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		UAnimBlueprint* FlyingAnim_BP;*/

	/*UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UMaterial* StoredMaterial = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UMaterialInstanceDynamic* DynamicMaterialInst = nullptr;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physical material")
		UPhysicalMaterial* PhysPlayerMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* PowerBarWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> PowerBarWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* PauseWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> PauseWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* SkipCameraPanWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> SkipCameraPanWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* FlyingScoreWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> FlyingScoreWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* ClimbingScoreWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> ClimbingScoreWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* WalkingScoreWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> WalkingScoreWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* GolfStrokesWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> GolfStrokesWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* RunnerTimerWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> RunnerTimerWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking variable")
		float walkMaxDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking variable")
		float walkTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing variable")
		bool climbingCanLaunch = false;

	//Sound effects
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* jumpSound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* golfHitSound = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundCue* pacmanPickupSound = nullptr;


	enum states
	{
		GOLF = 0,
		WALKING = 1,
		CLIMBING = 2,
		FLYING = 3,
		LEVEL_SELECT = 4,
		AWAITING_LEVELSELECT_INPUT = 5,
		PLINKO = 6,
		PACMAN = 7,
		RUNNER = 8,
		CREDITS = 9
	};
	UPROPERTY(BlueprintReadOnly)
		int state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Golf variable")
		float currentLaunchPower = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Golf variable")
		float maxLaunchPower = 11000.f;

	ADirectionIndicator* dirIndicator = nullptr;
	FActorSpawnParameters spawnInfo;
	UPROPERTY(EditAnywhere)
		TSubclassOf<ADirectionIndicator> ToSpawn;
	float distanceFromBall = 100.f;
	float indicatorStretch = 0.f;
	FVector indicatorColor = FVector(0.f, 0.f, 0.f);

	float cameraFadeTimer = 1.f;
	UCameraComponent* topDownCamera;
	AController* mController;

	void levelInit();
	void golfInit(bool playTransformParticles = true);
	void climbingInit(AActor *OtherActor, bool playTransformParticles = true);
	void flyingInit(AActor *OtherActor, bool playTransformParticles = true);

	bool isCharging = false;
	bool canLaunch = true;
	float launchPowerIncrement = 5000.f;
	float mouseX;
	float mouseY;
	float frameX = 1206.f;
	float frameY = 572.f;

	//Flying
	FVector position;
	FVector velocity;
	FVector acceleration;
	FVector gravity = FVector(0, 0, 0);
	float gravityZ = -1.5;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool hoverInAir = true;

	//FVector gravitation = FVector(0.f, 0.f, -400000.f);
	bool flyingGravityFlipped = false;
	bool easeGravityShift = false;
	float easeTimer = 0.f;

	//Climbing
	FVector debugMouseLine;
	float ratio = 0.f;
	FVector OActorForwardVector;
	FVector mousePositionClicked;
	FVector mousePositionReleased;
	FVector climbingCameraPosition;
	FRotator climbingCameraRotation;
	AClimbObject* currentClimbObject;
	TArray<AActor*> destroBlocks;
	FVector dirIndicatorLocation;

	void walkFunction(float deltaTime);
	void tickWalking(float DeltaTime);
	void jump();
	void applyForce(FVector Force);
	void updatePosition(float DeltaTime);
	void stopStrike();

	void WClicked();
	void WReleased();
	void AClicked();
	void AReleased();
	void SClicked();
	void SReleased();
	void DClicked();
	void DReleased();
	void spacebarPressed();
	void spacebarReleased();
	void setLMBPressed();
	void setLMBReleased();

	void mouseCameraPitch();
	void mouseCameraYaw();
	float cameraSpeed;

	void leftShiftPressed();
	void enterPressed();
	void scrollUp();
	void scrollDown();

	bool WPressed = false;
	bool APressed = false;
	bool SPressed = false;
	bool DPressed = false;
	bool LMBPressed = false;

	//WALKING DATA

	bool sphereTrace();
	bool lineTrace();
	void constructTransform(FVector hitLocation, FVector impactNormal);
	TArray<FHitResult> hitResults;
	TArray<FHitResult> lineTraceResults;
	FCollisionQueryParams traceParams;
	bool lineTraceHit = false;
	FVector impactPoint;
	FVector surfaceNormal;
	FTransform newRotationTransform;
	FTransform newTranslationTransform;
	float walkingDirection = 0.f;
	bool onPlatform = false;
	FVector platformOffset;
	float PhysVelPrevFrame;

	void lerpPerspective(FRotator springToRot, float springToLength, FRotator camToRot, float DeltaTime);
	bool bLerpingPerspective = false;
	FRotator currentRotation;

	bool shouldLaunch = false;
	float readyToLaunchTimer;

	float movementSpeed = 2000000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bValidInput = false;
	bool platformJump = false;
	void movementTransformation(float DeltaTime);
	AWorldSettings* mWorldSettings;
	bool jumpingNotReady = false;
	float jumpingCooldown;
	bool coyoteJumpAvailable = false;
	float coyoteTimer;


	//Scoring
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int flyingRestarts = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int climbingRestarts = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int walkingRestarts = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int strokeCounter = 0;

	//Death and respawning

	void respawnAtCheckpoint();
	void respawnAtCheckpointTick(float deltaTime);
	FVector SpawnPosition;
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRespawning = false;

	bool bStartRespawnCameraFade = false;
	float timeToCameraFadeEnd = 0.f;

	//savingMerge
	void confirmLevelSelection();
	void setLevelToOpen(FString Name);
	FString levelToOpen = TEXT("None");
	ALevelSelecter* currentLevelSelecter = nullptr;

	//animation control
	void animationControlTick(float DeltaTime);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bFlyingAnimShouldPlay = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool onGround = false;

	bool bRestartFlyingAnim = false;
	float flyingAnimTimer = 0.f;

	const float flyingAnimLength = 0.375f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float stretchRatio = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bClimbInAir = false;

	//Enter level camera pan
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCameraShouldPan;

	void cameraPanTick(float deltaTime);

	//Non-secret, non-levelselect levels should have four view targets (camera actors), tagged "Target0", "Target1", "Target2", "Target3"
	TArray<AActor*> viewTargets;
	float viewTargetBlendTime = 2.5f;
	float blendTimer = 0.f;
	int currentViewTarget = 0;
	bool newViewTargetSet = false;
	AActor* target = nullptr;
		

	//Debug and misc
	FString debugMouseX;
	FString debugMouseY;
	void debugMouse();
	void drawDebugObjectsTick();
	bool timerFunction(float timerLength, float DeltaTime);
	float clock = 0.f;
	
	void displayDialogue();
	bool dialogueHasPlayed = false;

	void setMeshVisibility();

	void pauseGame();

	bool mouseInputEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool orbitCamera = false;

	void openLevelOne();
	void openLevelTwo();
	void openLevelThree();
	void openLevelFour();
	void openLevelFive();
	void openLevelSix();
	void openLevelSeven();
	void openLevelEight();
	void openLevelNine();
	//bool onMoveablePlatform = false;

	UFUNCTION(BlueprintImplementableEvent)
		void switchDecalVisibility(const bool visible);

	UFUNCTION(BlueprintImplementableEvent)
		void playGolfTutorial();

	UFUNCTION(BlueprintImplementableEvent)
		void playWalkTutorial();

	UFUNCTION(BlueprintImplementableEvent)
		void playClimbTutorial();

	UFUNCTION(BlueprintImplementableEvent)
		void playFlyTutorial();

	UFUNCTION(BlueprintImplementableEvent)
		void playZoomTutorial();

	UFUNCTION(BlueprintImplementableEvent)
		void playPacmanTutorial();

	//UDecalComponent* decalShadow = nullptr;

	//TArray<FHitResult> decalPosition;

	//Dialogue
	UFUNCTION(BlueprintImplementableEvent)
		void printDialogue(const TArray<FString> &displayText);

	//Secret levels
	ASecretLevelManager* secretLevelManagerInstance = nullptr;
	bool bPlayingSecretLevel = false;
	FVector billiardsLaunchDirection;

	//Particle systems
	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* canLaunchReadyParticles = nullptr;
	bool canLaunchParticlesHaveActivated = true;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* trailParticles = nullptr;

	UPROPERTY(VisibleAnywhere)
		UParticleSystemComponent* transformParticles = nullptr;
};