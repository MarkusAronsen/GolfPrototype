// Fill out your copyright notice in the Description page of Project Settings.

#include "TranslatingGeometry2.h"

// Sets default values
ATranslatingGeometry2::ATranslatingGeometry2()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATranslatingGeometry2::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATranslatingGeometry2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorUpVector() * sin(position) * 10);
	position += 0.03;
	if (position > 2 * PI + PI)
	{
		position = PI;
	}
}

