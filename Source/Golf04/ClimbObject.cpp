// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbObject.h"


// Sets default values
AClimbObject::AClimbObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClimbObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClimbObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (ignored)
	{
		ignoreTimer += DeltaTime;
		if (ignoreTimer >= 1.f)
		{
			ignoreTimer = 0.f;
			ignored = false;
		}
	}*/
}

