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

	if (rotateRollPeriodically)
	{
		static float rotateTimer = 0.f;
		static float rotationSpeed = 0.4f;
		static FRotator lockedRotation = FRotator(0.f, 0.f, 0.f);
		static FRotator targetRotation = FRotator(90.f, 90.f, 90.f);
		static float lerpAlpha = 0.f;

		rotateTimer += DeltaTime;

		if (rotateTimer >= 3.5f)
		{
			SetActorRotation(FMath::Lerp(lockedRotation, targetRotation, lerpAlpha));
			lerpAlpha += DeltaTime * rotationSpeed;

			if (lerpAlpha >= 1.f)
			{
				lerpAlpha = 0.f;
				rotateTimer = 0.f;
				lockedRotation = targetRotation;
				targetRotation = targetRotation * 2.f;
			}
		}
	}

	if (rotateOneAxisConstantly)
	{
		static float angle = 0.f;
		static float rotationSpeed = 10.f;

		SetActorRotation(FRotator(0.f, 0.f, angle));
		angle += DeltaTime * rotationSpeed;
		if (angle > 360.f)
			angle = 1.f;
	}

	if (rotateNextAxisPeriodically)
	{
		static float rotateTimer = 0.f;
		static int modeSwitch = 1;
		static float angle = 0.f;
		static float rotationSpeed = 100.f;

		rotateTimer += DeltaTime;

		if (rotateTimer >= 3.5f)
		{
			switch (modeSwitch)
			{
			case 1:
				AddActorLocalRotation(FRotator(0, 0, DeltaTime * rotationSpeed));
				angle += DeltaTime * rotationSpeed;
				if (angle > 90.f)
				{
					angle = 0.f;
					rotateTimer = 0.f;
					modeSwitch = 2;
				}
				break;
			case 2:
				AddActorLocalRotation(FRotator(0, DeltaTime * rotationSpeed, 0));
				angle += DeltaTime * rotationSpeed;
				if (angle > 90.f)
				{
					angle = 0.f;
					rotateTimer = 0.f;
					modeSwitch = 3;
				}
				break;
			case 3:
				AddActorLocalRotation(FRotator(DeltaTime * rotationSpeed, 0, 0));
				angle += DeltaTime * rotationSpeed;
				if (angle > 90.f)
				{
					angle = 0.f;
					rotateTimer = 0.f;
					modeSwitch = 1;
				}
				break;
			}
		}
	}

	if (scaleUpAndDownPeriodically)
	{
		static float timeToScale = 0.f;
		static bool scaleUp = true;
		static float scaleXY = 0.f;
		static float scaleZ = 0.f;
		static float scaleSpeed = 10.f;

		timeToScale += DeltaTime;

		if (timeToScale > 3.f && scaleUp)
		{
			SetActorRelativeScale3D(FVector(scaleXY, scaleXY, scaleZ));
			scaleZ += DeltaTime * scaleSpeed * 0.5f;
			scaleXY += DeltaTime * scaleSpeed;
			if (scaleZ > 2.f)
			{
				scaleUp = false;
				timeToScale = 0.f;
			}
		}

		else if (timeToScale > 3.f && !scaleUp)
		{
			SetActorRelativeScale3D(FVector(scaleXY, scaleXY, scaleZ));
			scaleZ -= DeltaTime * scaleSpeed * 0.5f;
			scaleXY -= DeltaTime * scaleSpeed;
			if (scaleZ < 1.f)
			{
				scaleUp = true;
				timeToScale = 0.f;
			}
		}
	}

	if (translateUpAndDown)
	{
		static float position = PI;
		static float translationSpeed = 10.f;

		SetActorLocation(GetActorLocation() + GetActorUpVector() * sin(position) * 10);
		position += DeltaTime * translationSpeed;
		if (position > 2 * PI + PI)
		{
			position = PI;
		}
	}

	if (translateBackAndForth)
	{
		static float position = PI;
		static float translationSpeed = 10.f;

		SetActorLocation(GetActorLocation() + GetActorForwardVector() * sin(position) * 10);
		position += DeltaTime * translationSpeed;
		if (position > 2 * PI + PI)
		{
			position = PI;
		}
	}

}

