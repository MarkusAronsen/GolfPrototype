// Fill out your copyright notice in the Description page of Project Settings.

#include "ObjectFloater.h"

// Sets default values
AObjectFloater::AObjectFloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AObjectFloater::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AObjectFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	offset += DeltaTime * floatSpeed;

	SetActorLocation(FVector(GetActorLocation().X + (sin(offset) * Xoffset), GetActorLocation().Y + (sin(offset) * Yoffset), GetActorLocation().Z + (sin(offset) * Zoffset)));

	if (offset > 2.f * PI)
		offset = 0.f;

	if (rotatePitch)
		AddActorLocalRotation(FRotator(rotateSpeed, 0, 0));
	if (rotateYaw)
		AddActorLocalRotation(FRotator(0, rotateSpeed, 0));
	if (rotateRoll)
		AddActorLocalRotation(FRotator(0, 0, rotateSpeed));
}

