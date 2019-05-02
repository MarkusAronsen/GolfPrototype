// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Camera/CameraComponent.h"


#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GolfSaveGame.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Goal.generated.h"

class AGolfBall;
class AGolfGameInstance;

UCLASS()
class GOLF04_API AGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	USphereComponent* CollisionBox = nullptr;
	USphereComponent* OuterCollisionBox = nullptr;
	UStaticMeshComponent* Mesh = nullptr;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapOuter(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlapOuter(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* LevelFinishedWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> LevelFinishedWidget_BP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString levelToOpen;

	bool elevate = false;
	float elevateValue = 0.f;
	bool descend = false;

	float initialZ = 0.f;

	void saveLevelData();

	int levelPerformance = 0;
	float levelTimeElapsed;

	FString levelName;

	float settleTimer = 0.f;
	bool startSettleTimer = false;


	UParticleSystemComponent* goalParticles = nullptr;
};