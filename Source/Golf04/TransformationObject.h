// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define PI 3.14159265//358//9793238462643383279502884
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransformationObject.generated.h"

UCLASS()
class GOLF04_API ATransformationObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransformationObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool rotateAllAxisPeriodically;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool rotateOneAxisConstantly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool rotateNextAxisPeriodically;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool scaleUpAndDownPeriodically;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool translateUpAndDown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool translateBackAndForth;

};
