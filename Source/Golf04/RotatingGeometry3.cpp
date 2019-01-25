// Fill out your copyright notice in the Description page of Project Settings.

#include "RotatingGeometry3.h"

// Sets default values
ARotatingGeometry3::ARotatingGeometry3()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotatingGeometry3::BeginPlay()
{
	Super::BeginPlay();
	LockedRotation = GetActorRotation();
}

// Called every frame
void ARotatingGeometry3::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorRotation(FRotator(0, 0, angle));
	angle++;
	if (angle > 360)
		angle = 1;
}

