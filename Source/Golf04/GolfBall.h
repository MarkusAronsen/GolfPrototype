// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ClimbObject.h"
#include "SomersaultObject.h"
#include "Goal.h"
#include "LegsPUp.h"
#include "WingsPUp.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Core/Public/Math/Rotator.h"

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

	FRotator LockedClimbRotation;
	FVector LockedClimbPosition;
	FVector SomersaultCenter;
	FVector PrevPos;
	FVector FlyingVector;
	float Gravity = -1.f;
	float Ascend = 0.f;
	float Acceleration = 0.f;

	float radius = 100.f;
	float degree = PI * 1.5;
	bool toLaunch = false;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		UShapeComponent * CollisionBox = nullptr;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent * Mesh = nullptr;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking variable")
		float walkMaxDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking variable")
		float walkTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level variable")
		int strokeCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Walking variable")
		bool isWalking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing variable")
		bool isClimbing = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Somersault variable")
		//bool isSomersaulting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying variable")
		bool isFlying = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing variable")
		bool climbingCanLaunch = false;

	enum states
	{
		GOLF = 0,
		WALKING = 1,
		CLIMBING = 2,
		FLYING = 3
	};
	int state;

	float currentLaunchPower = 0.f;
	float maxLaunchPower = 7000.f;
	bool isCharging = false;
	bool canLaunch = true;
	float launchPowerIncrement = 30.f;

	void walkFunction(float deltaTime);
	//void launchBall();
	//void orbit();
	//void launchReady();
	void flying(float deltaTime);
	void flappyAscend();

	void setW();
	void setA();
	void setS();
	void setD();
	void setLMBPressed();
	void setLMBReleased();

	bool WPressed = false;
	bool APressed = false;
	bool SPressed = false;
	bool DPressed = false;
	bool LMBPressed = false;

};
