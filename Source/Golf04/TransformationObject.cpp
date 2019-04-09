// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformationObject.h"

// Sets default values
ATransformationObject::ATransformationObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransformationObject::BeginPlay()
{
	Super::BeginPlay();
	
	startPos = GetActorLocation();
}

// Called every frame
void ATransformationObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (rotateNextAxisPeriodically)
	{
		rotateTimer1 += DeltaTime;

		if (rotateTimer1 >= nextAxisInterval)
		{
			nextAxisStartAngle += DeltaTime * nextAxisRotationSpeed;

			switch (modeSwitch1)
			{
			case 1:
				AddActorLocalRotation(FRotator(0, 0, DeltaTime * nextAxisRotationSpeed));
				if (nextAxisStartAngle > 90.f)
				{
					nextAxisStartAngle = 0.f;
					rotateTimer1 = 0.f;
					modeSwitch1 = 2;
				}
				break;
			case 2:
				AddActorLocalRotation(FRotator(0, DeltaTime * nextAxisRotationSpeed, 0));
				if (nextAxisStartAngle > 90.f)
				{
					nextAxisStartAngle = 0.f;
					rotateTimer1 = 0.f;
					modeSwitch1 = 3;
				}
				break;
			case 3:
				AddActorLocalRotation(FRotator(DeltaTime * nextAxisRotationSpeed, 0, 0));
				if (nextAxisStartAngle > 90.f)
				{
					nextAxisStartAngle = 0.f;
					rotateTimer1 = 0.f;
					modeSwitch1 = 1;
				}
				break;
			}
		}
	}

	if (scaleUpAndDownPeriodically)
	{
		scaleUpAndDownTime += DeltaTime * 0.2f;

		if (scaleUpAndDownTime > scaleUpAndDownIntervalLength && interval)
		{
			SetActorScale3D(FMath::Lerp(FVector(ScaleDownXYMultiplier, ScaleDownXYMultiplier, 1.f), FVector(ScaleUpXYMultiplier, ScaleUpXYMultiplier, 1.f), lerpAlpha2));
			lerpAlpha2 += DeltaTime * ScaleUpAndDownRate;
			if (lerpAlpha2 >= 1.f)
			{
				interval = false;
				scaleUpAndDownTime = 0.f;
				lerpAlpha2 = 0.f;
			}
		}

		else if (scaleUpAndDownTime > scaleUpAndDownIntervalLength && !interval)
		{
			SetActorScale3D(FMath::Lerp(FVector(ScaleUpXYMultiplier, ScaleUpXYMultiplier, 1.f), FVector(ScaleDownXYMultiplier, ScaleDownXYMultiplier, 1.f), lerpAlpha2));
			lerpAlpha2 += DeltaTime * ScaleUpAndDownRate;
			if (lerpAlpha2 >= 1.f)
			{
				interval = true;
				scaleUpAndDownTime = 0.f;
				lerpAlpha2 = 0.f;
			}
		}
	}
	
	if (rotateAllAxisPeriodically)
	{
		rotateTimer3 += DeltaTime;
		AddActorLocalRotation(FRotator(DeltaTime * rotationSpeed3, DeltaTime * rotationSpeed3, DeltaTime * rotationSpeed3));

		if (rotateTimer3 >= interval3)
		{
			angle3 += DeltaTime * rotationSpeed3;
			if (angle3 > 90.f)
			{
				angle3 = 0.f;
				rotateTimer3 = 0.f;
			}
		}
	}

	if (rotateOneAxisConstantly)
	{
		AddActorLocalRotation(FRotator(0.f, 0.f, DeltaTime * rotationSpeed4));
		angle4 += DeltaTime * rotationSpeed4;
		if (angle4 > 360.f)
			angle4 = 1.f;
	}

	if (translateBackAndForth)
	{
		AddActorWorldOffset(GetActorForwardVector() * sin(position5) * rangeXY);
		position5 += DeltaTime * translationSpeed5;
		if (position5 > 2 * PI + PI)
		{
			position5 = PI;
		}
	}
	
	if (translateUpAndDown)
	{
		AddActorWorldOffset(GetActorUpVector() * sin(position6) * rangeZ);
		position6 += DeltaTime * translationSpeed6;
		if (position6 > 2 * PI + PI)
		{
			position6 = PI;
		}
	}

	if (bIsFalling)
	{
		AddActorLocalOffset(FVector(0.f, 0.f, -DeltaTime * 1000.f));
		if (GetActorLocation().Z < startPos.Z - 3000.f)
		{
			SetActorLocation(startPos);
			bIsFalling = false;
		}
	}
}

