// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerFloor.h"

// Sets default values
ARunnerFloor::ARunnerFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARunnerFloor::BeginPlay()
{
	Super::BeginPlay();

	s_NumSpawned++;
}

// Called every frame
void ARunnerFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	s_MoveSpeed += DeltaTime * 0.1;

	UE_LOG(LogTemp, Warning, TEXT("s_MoveSpeed: %f"), s_MoveSpeed);

	/*if (GetActorLocation().Z < 0)
	{
		SetActorLocation(GetActorLocation() + FVector(0, 0, 1) * DeltaTime * moveSpeed);
	}*/
	//else
	{
		//if (GetActorLocation().Z != 0)
			//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0));

		SetActorLocation(GetActorLocation() + FVector(-1, 0, 0) * DeltaTime * s_MoveSpeed);
	}
		
	if (GetActorLocation().X < -450)
	{
		spawnNewFloor();
		Destroy();
	}
}

void ARunnerFloor::spawnNewFloor()
{
	if (s_NumSpawned % 25 == 0)
		GetWorld()->SpawnActor<AFallingRunnerFloor>(FallingFloorSpawn, FVector(2000, GetActorLocation().Y, 0), FRotator::ZeroRotator);
	else
		GetWorld()->SpawnActor<ARunnerFloor>(RunnerFloorSpawn, FVector(2000, GetActorLocation().Y, 0), FRotator::ZeroRotator);

	if (s_NumSpawned > 300)
		s_NumSpawned = 0;
		

	UE_LOG(LogTemp, Warning, TEXT("s_NumSpawned: %i"), s_NumSpawned);
	
}