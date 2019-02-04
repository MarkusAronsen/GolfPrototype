// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClimbObject.h"
#include "Goal.h"

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
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* mCamera = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* mSpringArm = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* mCollisionBox = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mMesh = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UMaterial* StoredMaterial = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class UMaterialInstanceDynamic* DynamicMaterialInst = nullptr;

	UPROPERTY(Category = "Widget", EditAnywhere, BlueprintReadWrite)
		UUserWidget* PowerBarWidget = nullptr;

	UPROPERTY(Category = "Widget", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> PowerBarWidget_BP;

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
		LEVEL_SELECT = 4
	};
	int state;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Golf variable")
		float currentLaunchPower = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Golf variable")
		float maxLaunchPower = 10000.f;

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
	FVector gravitation = FVector(0.f, 0.f, -400000.f);

	void walkFunction(float deltaTime);
	void tickWalking(float DeltaTime);
	void jump();
	void applyForce(FVector Force);
	void updatePosition();
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
	void setLMBPressed();
	void setLMBReleased();
	void mouseCameraPitch();
	void mouseCameraYaw();
	void leftShiftPressed();
	void scrollUp();
	void scrollDown();

	bool WPressed = false;
	bool APressed = false;
	bool SPressed = false;
	bool DPressed = false;
	bool LMBPressed = false;

	bool sphereTrace();
	TArray<FHitResult> hitResults;
	bool onGround = false;

	void lerpPerspective(FRotator springToRot, float springToLength, FRotator camToRot, float DeltaTime);
	float lerpTimer = 0.f;
	float lerpTime = 5.f;
	FRotator currentRotation;

	float movementSpeed;

	//Debug purposes
	FString debugMouseX;
	FString debugMouseY;
	void debugMouse();
	void drawDebugObjectsTick();
};
