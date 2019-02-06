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

	if (rotateAllAxisPeriodically)
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
				targetRotation = FRotator(targetRotation.Roll + 90.f, targetRotation.Pitch + 90.f, targetRotation.Yaw + 90.f);
				SetActorRotation(lockedRotation);
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
				angle += DeltaTime * rotationSpeed;
				if (angle > 90.f)
				{
					angle = 0.f;
					rotateTimer = 0.f;
					modeSwitch = 2;
				}
				break;
			case 2:
				angle += DeltaTime * rotationSpeed;
				if (angle > 90.f)
				{
					angle = 0.f;
					rotateTimer = 0.f;
					modeSwitch = 3;
				}
				break;
			case 3:
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
		static float scaleXY = 0.f;
		static float scaleZ = 0.f;
		static float scaleSpeed = 0.35f;

		timeToScale += DeltaTime;

		scaleZ = DeltaTime * scaleSpeed * 0.2f;
		scaleXY = DeltaTime * scaleSpeed;

		if (timeToScale > 3.f && scaleUp)
		{
			SetActorScale3D(FVector(GetActorScale().X + scaleXY, GetActorScale().Y + scaleXY, GetActorScale().Z + scaleZ));
			if (timeToScale > 6.f)
		{
				scaleUp = false;
				timeToScale = 0.f;
			}
		}

		if (timeToScale > 3.f && !scaleUp)
		{
			SetActorScale3D(FVector(GetActorScale().X - scaleXY, GetActorScale().Y - scaleXY, GetActorScale().Z - scaleZ));
			if (timeToScale > 6.f)
			{
				scaleUp = true;
				timeToScale = 0.f;
			}
		}
	}
	if (rotateAllAxisPeriodically)
	{
		static float rotateTimer = 0.f;
		static FRotator LockedRotation = FRotator(0.f, 0.f, 0.f);
		static float angle = 0.f;
		static float rotationSpeed = 100.f;
		static int rotationCount = 0;

		rotateTimer += DeltaTime;

		SetActorRotation(LockedRotation + FRotator(angle, angle, angle));

		if (rotateTimer >= 3.5f)
		{
			angle += DeltaTime * rotationSpeed;
			if (angle > 90.f)
			{
				rotationCount++;
				angle = 0.f;
				rotateTimer = 0.f;
				LockedRotation = FRotator(90.f * rotationCount, 90.f * rotationCount, 90.f * rotationCount);
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
}

