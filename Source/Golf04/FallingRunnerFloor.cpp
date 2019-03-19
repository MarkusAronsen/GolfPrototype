// Fill out your copyright notice in the Description page of Project Settings.

#include "FallingRunnerFloor.h"

// Sets default values
AFallingRunnerFloor::AFallingRunnerFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFallingRunnerFloor::BeginPlay()
{
	Super::BeginPlay();

	s_NumSpawned++;
}

// Called every frame
void AFallingRunnerFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//s_moveSpeed += DeltaTime * 0.1;

	/*if (GetActorLocation().Z < 0)
	{
		SetActorLocation(GetActorLocation() + FVector(0, 0, 1) * DeltaTime * moveSpeed);
	}*/
	//else
	{
		//if (GetActorLocation().Z != 0)
			//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0));

		if (GetActorLocation().X < 500)
		{
			SetActorLocation(GetActorLocation() + FVector(-1, 0, -3) * DeltaTime * s_MoveSpeed);
		}
		else
			SetActorLocation(GetActorLocation() + FVector(-1, 0, 0) * DeltaTime * s_MoveSpeed);
	}

	if (GetActorLocation().X < -450)
	{
		UE_LOG(LogTemp, Warning, TEXT("Removed falling floor"));
		//spawnNewFloor();
		Destroy();
	}
}

void AFallingRunnerFloor::spawnNewFloor()
{
	GetWorld()->SpawnActor<ARunnerFloor>(RunnerFloorSpawn, FVector(2000, GetActorLocation().Y, 0), FRotator::ZeroRotator);

	UE_LOG(LogTemp, Warning, TEXT("s_NumSpawned: %i"), s_NumSpawned);
}