// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClimbObject.h"
#include "Goal.h"
#include "GolfSaveGame.h"
#include "Checkpoint.h"
#include "GolfGameInstance.h"
#include "TransformationObject.h"
#include "SecretLevelManager.h"

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

	//UPROPERTY(Category = "Component")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* mCamera = nullptr;

	//UPROPERTY(Category = "Component")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* mSpringArm = nullptr;

	//UPROPERTY(Category = "Component")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* mCollisionBox = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mWingsMeshLeft = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mWingsMeshRight = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mLegsMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")//, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* mArmsMesh = nullptr;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking variable")
		float walkMaxDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking variable")
		float walkTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level variable")
		int strokeCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing variable")
		bool climbingCanLaunch = false;


	enum states
	{
		GOLF = 0,
		WALKING = 1,
		CLIMBING = 2,
		FLYING = 3,
		LEVEL_SELECT = 4,
		AWAITING_LEVELSELECT_INPUT = 5,
		PLINKO = 6
	};
	int state;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Golf variable")
		float currentLaunchPower = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Golf variable")
		float maxLaunchPower = 10000.f;

	float cameraFadeTimer = 1.f;
	UCameraComponent* topDownCamera;
	AController* mController;

	void levelInit();
	void golfInit();
	void climbingInit(AActor *OtherActor);
	void flyingInit(AActor *OtherActor);

	bool isCharging = false;
	bool canLaunch = true;
	float launchPowerIncrement = 5000.f;
	float mouseX;
	float mouseY;
	float frameX = 1206.f;
	float frameY = 572.f;
	FVector OActorForwardVector;
	FVector mousePositionClicked;
	FVector mousePositionReleased;
	FVector climbingCameraPosition;
	FRotator climbingCameraRotation;
	FVector position;
	FVector velocity;
	FVector acceleration;
	FVector gravity = FVector(0, 0, -1.5f);
	//FVector gravitation = FVector(0.f, 0.f, -400000.f);

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
	void scrollUp();
	void scrollDown();

	bool WPressed = false;
	bool APressed = false;
	bool SPressed = false;
	bool DPressed = false;
	bool LMBPressed = false;

	bool sphereTrace();
	bool lineTrace();
	void constructTransform(FVector hitLocation, FVector impactNormal);
	TArray<FHitResult> hitResults;
	TArray<FHitResult> lineTraceResults;
	FCollisionQueryParams traceParams;
	bool onGround = false;
	bool alignWithSurface = false;
	FVector impactPoint;
	FVector surfaceNormal;
	FTransform newRotationTransform;
	FTransform newTranslationTransform;
	float walkingDirection = 0.f;
	bool onPlatform = false;
	FVector platformOffset;
	float PhysVelPrevFrame;

	void lerpPerspective(FRotator springToRot, float springToLength, FRotator camToRot, float DeltaTime);
	float lerpTimer = 0.f;
	float lerpTime = 10.f;
	FRotator currentRotation;

	float movementSpeed = 2000000.f;
	bool bValidInput = false;
	bool platformJump = false;
	void movementTransformation(float DeltaTime);
	AWorldSettings* mWorldSettings;

	//Death and respawning
	void respawnAtCheckpoint();
	void respawnAtCheckpointTick(float deltaTime);
	FVector SpawnPosition;
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
		bool bIsWalking = false;

	bool bRestartFlyingAnim = false;
	float flyingAnimTimer = 0.f;

	const float flyingAnimLength = 0.375f;

	//Enter level camera pan
	bool bCameraShouldPan;
	void cameraPanTick();

	//Debug and misc
	FString debugMouseX;
	FString debugMouseY;
	void debugMouse();
	void drawDebugObjectsTick();
	bool timerFunction(float timerLength, float DeltaTime);
	float clock = 0.f;
	
	void printLoadedGame();

	void setMeshVisibility();

	void pauseGame();

	//Secret levels
	ASecretLevelManager* secretLevelManagerInstance = nullptr;
	bool bPlayingSecretLevel = false;
};
