// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingObstacle.h"


// Sets default values
AFlyingObstacle::AFlyingObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlyingObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

