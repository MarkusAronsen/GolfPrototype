// Fill out your copyright notice in the Description page of Project Settings.

#include "PacmanGhost.h"

// Sets default values
APacmanGhost::APacmanGhost()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacmanGhost::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APacmanGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (directionBuffer != -1 && pathNode && (GetActorLocation() - pathNode->GetActorLocation()).Size() < 20)
	{
		switch (directionBuffer)
		{
		case golf::directions::UP:
			direction = FVector(1, 0, 0);
			break;
		case golf::directions::DOWN:
			direction = FVector(-1, 0, 0);
			break;
		case golf::directions::LEFT:
			direction = FVector(0, -1, 0);
			break;
		case golf::directions::RIGHT:
			direction = FVector(0, 1, 0);
			break;

		default:
			break;
		}
		directionBuffer = -1;
	}

	SetActorLocation(GetActorLocation() + direction * 300);

	UE_LOG(LogTemp, Warning, TEXT(""))
}

