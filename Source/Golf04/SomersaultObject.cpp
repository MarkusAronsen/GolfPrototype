// Fill out your copyright notice in the Description page of Project Settings.

#include "SomersaultObject.h"


// Sets default values
ASomersaultObject::ASomersaultObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASomersaultObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASomersaultObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

