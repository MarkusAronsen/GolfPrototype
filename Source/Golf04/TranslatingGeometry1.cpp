// Fill out your copyright notice in the Description page of Project Settings.

#include "TranslatingGeometry1.h"

// Sets default values
ATranslatingGeometry1::ATranslatingGeometry1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATranslatingGeometry1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATranslatingGeometry1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//SetActorLocation(GetActorLocation() + FVector(sin(position) * 10, 0, 0));
	SetActorLocation(GetActorLocation() + GetActorForwardVector() * sin(position) * 10);

	position += 0.03;
	if (position > 2 * PI + PI)
	{
		position = PI;
	}

}

