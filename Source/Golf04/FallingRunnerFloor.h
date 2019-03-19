// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "RunnerFloor.h"
#include "SecretLevelManager.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingRunnerFloor.generated.h"

UCLASS()
class GOLF04_API AFallingRunnerFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFallingRunnerFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//float moveSpeed = 500.f;

	void spawnNewFloor();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARunnerFloor> RunnerFloorSpawn;

	class ASecretLevelManager* secretLevelManagerInstance = nullptr;
};
