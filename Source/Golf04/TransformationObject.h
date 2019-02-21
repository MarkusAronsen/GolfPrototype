// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define PI 3.14159265//358//9793238462643383279502884

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
		bool rotateNextAxisPeriodically;

	float rotateTimer1 = 0.f;
	int modeSwitch1 = 1;
	float angle1 = 0.f;
	float rotationSpeed1 = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool scaleUpAndDownPeriodically;

	bool scaleUp2 = true;
	float lerpAlpha2 = 0.f;
	float XYScaleDown2 = 1.f;
	float XYScaleUp2 = 2.f;
	float scaleSpeed2 = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scaling sequence")
		float timeToScale2 = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool rotateAllAxisPeriodically;

	float rotateTimer3 = 0.f;
	float angle3 = 0.f;
	float rotationSpeed3 = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool rotateOneAxisConstantly;

	float angle4 = 0.f;
	float rotationSpeed4 = 20.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool translateBackAndForth;

	float translationSpeed5 = 0.5f;
	float position5 = PI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving platform range")
		float rangeXY = 12.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transformation mode")
		bool translateUpAndDown;

	float position6 = PI;
	float translationSpeed6 = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving platform range")
		float rangeZ = 12.f;
};
