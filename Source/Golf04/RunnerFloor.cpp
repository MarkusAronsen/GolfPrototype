// Fill out your copyright notice in the Description page of Project Settings.

#include "RunnerFloor.h"
#include "SecretLevelManager.h"
#include "FallingRunnerFloor.h"

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
	TArray<AActor*> secretLevelManager;
	UGameplayStatics::GetAllActorsOfClass(this, ASecretLevelManager::StaticClass(), secretLevelManager);
	if (secretLevelManager.Num() > 0)
		secretLevelManagerInstance = Cast<ASecretLevelManager>(secretLevelManager[0]);
	else
		UE_LOG(LogTemp, Warning, TEXT("Secret level manager not found"));


	//s_NumSpawned++;
}

// Called every frame
void ARunnerFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//s_MoveSpeed += DeltaTime * 0.1;

	//UE_LOG(LogTemp, Warning, TEXT("s_MoveSpeed: %f"), s_MoveSpeed);		

	/*if (GetActorLocation().Z < 0)
	{
		SetActorLocation(GetActorLocation() + FVector(0, 0, 1) * DeltaTime * moveSpeed);
	}*/
	//else
	{
		//if (GetActorLocation().Z != 0)
			//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 0));

		SetActorLocation(GetActorLocation() + FVector(-1, 0, 0) * DeltaTime * secretLevelManagerInstance->runnerMoveSpeed);
	}
		
	if (GetActorLocation().X < -500)
	{
		spawnNewFloor();
		Destroy();
	}
}

void ARunnerFloor::spawnNewFloor()
{
	if (secretLevelManagerInstance->objectsSpawned % 27 == 0 && secretLevelManagerInstance->objectsSpawned != 0)
		GetWorld()->SpawnActor<AFallingRunnerFloor>(FallingFloorSpawn, FVector(4000, GetActorLocation().Y, 0), FRotator::ZeroRotator);
	else
		GetWorld()->SpawnActor<ARunnerFloor>(RunnerFloorSpawn, FVector(4000, GetActorLocation().Y, 0), FRotator::ZeroRotator);

	//if (secretLevelManagerInstance->objectsSpawned > 300)
		//secretLevelManagerInstance->objectsSpawned = 0;
		
	secretLevelManagerInstance->objectsSpawned++;
	UE_LOG(LogTemp, Warning, TEXT("s_NumSpawned: %i"), secretLevelManagerInstance->objectsSpawned);
}