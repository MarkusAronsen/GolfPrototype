// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RunnerFloor.generated.h"

//static int s_NumSpawned = 0;
//static float s_MoveSpeed = 300;

class ASecretLevelManager;
class AFallingRunnerFloor;

UCLASS()
class GOLF04_API ARunnerFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARunnerFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//float moveSpeed = 500.f;

	void spawnNewFloor();

	UPROPERTY(EditAnywhere)
		TSubclassOf<ARunnerFloor> RunnerFloorSpawn;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AFallingRunnerFloor> FallingFloorSpawn;

	class ASecretLevelManager* secretLevelManagerInstance = nullptr;
};
