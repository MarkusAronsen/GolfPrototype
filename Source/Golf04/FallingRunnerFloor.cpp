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

	TArray<AActor*> secretLevelManager;
	UGameplayStatics::GetAllActorsOfClass(this, ASecretLevelManager::StaticClass(), secretLevelManager);
	if (secretLevelManager.Num() > 0)
		secretLevelManagerInstance = Cast<ASecretLevelManager>(secretLevelManager[0]);
	else
		UE_LOG(LogTemp, Warning, TEXT("Secret level manager not found"));

	//s_NumSpawned++;
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
			SetActorLocation(GetActorLocation() + FVector(-1, 0, -3) * DeltaTime * secretLevelManagerInstance->runnerMoveSpeed);
		}
		else
			SetActorLocation(GetActorLocation() + FVector(-1, 0, 0) * DeltaTime * secretLevelManagerInstance->runnerMoveSpeed);
	}

	if (GetActorLocation().X < -500)
	{
		UE_LOG(LogTemp, Warning, TEXT("Removed falling floor"));
		//spawnNewFloor();
		Destroy();
	}
}

void AFallingRunnerFloor::spawnNewFloor()
{
	GetWorld()->SpawnActor<ARunnerFloor>(RunnerFloorSpawn, FVector(4000, GetActorLocation().Y, 0), FRotator::ZeroRotator);
}