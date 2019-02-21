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
	
	
}

// Called every frame
void ATransformationObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (rotateNextAxisPeriodically)
	{
		rotateTimer1 += DeltaTime;

		if (rotateTimer1 >= 3.5f)
		{
			angle1 += DeltaTime * rotationSpeed1;

			switch (modeSwitch1)
			{
			case 1:
				AddActorLocalRotation(FRotator(0, 0, DeltaTime * rotationSpeed1));
				if (angle1 > 90.f)
				{
					angle1 = 0.f;
					rotateTimer1 = 0.f;
					modeSwitch1 = 2;
				}
				break;
			case 2:
				AddActorLocalRotation(FRotator(0, DeltaTime * rotationSpeed1, 0));
				if (angle1 > 90.f)
				{
					angle1 = 0.f;
					rotateTimer1 = 0.f;
					modeSwitch1 = 3;
				}
				break;
			case 3:
				AddActorLocalRotation(FRotator(DeltaTime * rotationSpeed1, 0, 0));
				if (angle1 > 90.f)
				{
					angle1 = 0.f;
					rotateTimer1 = 0.f;
					modeSwitch1 = 1;
				}
				break;
			}
		}
	}

	if (scaleUpAndDownPeriodically)
	{
		timeToScale2 += DeltaTime * 0.2f;

		if (timeToScale2 > 3.f && scaleUp2)
		{
			SetActorScale3D(FMath::Lerp(FVector(XYScaleDown2, XYScaleDown2, 1.f), FVector(XYScaleUp2, XYScaleUp2, 1.f), lerpAlpha2));
			lerpAlpha2 += DeltaTime * scaleSpeed2;
			if (lerpAlpha2 >= 1.f)
			{
				scaleUp2 = false;
				timeToScale2 = 0.f;
				lerpAlpha2 = 0.f;
			}
		}

		else if (timeToScale2 > 3.f && !scaleUp2)
		{
			SetActorScale3D(FMath::Lerp(FVector(XYScaleUp2, XYScaleUp2, 1.f), FVector(XYScaleDown2, XYScaleDown2, 1.f), lerpAlpha2));
			lerpAlpha2 += DeltaTime * scaleSpeed2;
			if (lerpAlpha2 >= 1.f)
			{
				scaleUp2 = true;
				timeToScale2 = 0.f;
				lerpAlpha2 = 0.f;
			}
		}
	}
	
	if (rotateAllAxisPeriodically)
	{
		rotateTimer3 += DeltaTime;
		AddActorLocalRotation(FRotator(DeltaTime * rotationSpeed3, DeltaTime * rotationSpeed3, DeltaTime * rotationSpeed3));

		if (rotateTimer3 >= 20.f)
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
}

