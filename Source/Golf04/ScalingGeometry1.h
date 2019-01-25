// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScalingGeometry1.generated.h"

UCLASS()
class GOLF04_API AScalingGeometry1 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScalingGeometry1();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float timeToScale = 0.f;
	float scaleZ = 1.f;
	float scaleXY = 1.f;

	bool scaleUp = true;
};
