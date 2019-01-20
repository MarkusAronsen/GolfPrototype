// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingGeometry2.h"

// Sets default values
ARotatingGeometry2::ARotatingGeometry2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotatingGeometry2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotatingGeometry2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	rotateTimer += DeltaTime;

	if (rotateTimer >= 3.5f)
	{
		switch (modeSwitch)
		{
		case 1:
			SetActorRotation(FRotator(0, 0, angle));
			angle++;
			if (angle > 90)
			{
				angle = 0;
				rotateTimer = 0.f;
				modeSwitch = 2;
			}
				break;
		case 2:
			SetActorRotation(FRotator(0, angle, 0));
			angle++;
			if (angle > 90)
			{
				angle = 0;
				rotateTimer = 0.f;
				modeSwitch = 3;
			}
			break;
		case 3:
			SetActorRotation(FRotator(angle, 0, 0));
			angle++;
			if (angle > 90)
			{
				angle = 0;
				rotateTimer = 0.f;
				modeSwitch = 1;
			}
			break;
			}
	}
}

