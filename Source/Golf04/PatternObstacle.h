// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatternObstacle.generated.h"

UCLASS()
class GOLF04_API APatternObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatternObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		float delay = 0.f;

	float delayTimer = 0.f;

	bool active = false;

	FVector initialLocation;

	//Pattern 1 -> forward, down, backward, up
	FVector prevLocation1;

	UPROPERTY(EditAnywhere)
		bool pattern1 = false;

	UPROPERTY(EditAnywhere)
		float pattern1Speed = 1000.f;

	UPROPERTY(EditAnywhere)
		float pattern1Range = 1000.f;

	float pattern1RangeCounter = 0.f;

	bool pattern1M1 = true;
	bool pattern1M2 = false;
	bool pattern1M3 = false;
	bool pattern1M4 = false;
	//-----------------------

	//Pattern 2 -> down, backward, up, forward

	FVector prevLocation2;

	UPROPERTY(EditAnywhere)
		bool pattern2 = false;

	UPROPERTY(EditAnywhere)
		float pattern2Speed = 1000.f;

	UPROPERTY(EditAnywhere)
		float pattern2Range = 1000.f;

	float pattern2RangeCounter = 0.f;

	bool pattern2M1 = true;
	bool pattern2M2 = false;
	bool pattern2M3 = false;
	bool pattern2M4 = false;
	//-----------------------

	//Pattern 3 -> backward, up, forward, down

	FVector prevLocation3;

	UPROPERTY(EditAnywhere)
		bool pattern3 = false;

	UPROPERTY(EditAnywhere)
		float pattern3Speed = 1000.f;

	UPROPERTY(EditAnywhere)
		float pattern3Range = 1000.f;

	float pattern3RangeCounter = 0.f;

	bool pattern3M1 = true;
	bool pattern3M2 = false;
	bool pattern3M3 = false;
	bool pattern3M4 = false;

	//--------------------------

	//Pattern 4 -> up, forward, down, backward

	FVector prevLocation4;

	UPROPERTY(EditAnywhere)
		bool pattern4 = false;

	UPROPERTY(EditAnywhere)
		float pattern4Speed = 1000.f;

	UPROPERTY(EditAnywhere)
		float pattern4Range = 1000.f;

	float pattern4RangeCounter = 0.f;

	bool pattern4M1 = true;
	bool pattern4M2 = false;
	bool pattern4M3 = false;
	bool pattern4M4 = false;

	//-----------------------------------

	//Pattern 5 -> forward, right, backward, left
	FVector prevLocation5;

	UPROPERTY(EditAnywhere)
		bool pattern5 = false;

	UPROPERTY(EditAnywhere)
		float pattern5Speed = 1000.f;

	UPROPERTY(EditAnywhere)
		float pattern5Range = 1000.f;

	float pattern5RangeCounter = 0.f;

	bool pattern5M1 = true;
	bool pattern5M2 = false;
	bool pattern5M3 = false;
	bool pattern5M4 = false;
	//-----------------------

	//Pattern 6 -> backward, left, forward, right
	FVector prevLocation6;

	UPROPERTY(EditAnywhere)
		bool pattern6 = false;

	UPROPERTY(EditAnywhere)
		float pattern6Speed = 1000.f;

	UPROPERTY(EditAnywhere)
		float pattern6Range = 1000.f;

	float pattern6RangeCounter = 0.f;

	bool pattern6M1 = true;
	bool pattern6M2 = false;
	bool pattern6M3 = false;
	bool pattern6M4 = false;
	//-----------------------


};
