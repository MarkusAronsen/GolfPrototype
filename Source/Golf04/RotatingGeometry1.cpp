// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingGeometry1.h"

// Sets default values
ARotatingGeometry1::ARotatingGeometry1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotatingGeometry1::BeginPlay()
{
	Super::BeginPlay();

	LockedRotation = GetActorRotation();
}

// Called every frame
void ARotatingGeometry1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	rotateTimer += DeltaTime;

	if (rotateTimer >= 3.5f)
	{
		SetActorRotation(LockedRotation + FRotator(angle, angle, angle));
		angle++;
		if (angle > 90)
		{
			angle = 0;
			rotateTimer = 0.f;
			LockedRotation = GetActorRotation();
		}
	}
}

