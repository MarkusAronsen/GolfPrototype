// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

class AGolfBall;
class AGolfSaveGame;

UCLASS()
class GOLF04_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool elevate = false;
	float elevateValue = 0.f;
	bool descend = false;
	float initialZ = 0.f;

	USphereComponent* CollisionBox = nullptr;
	USphereComponent* OuterCollisionBox = nullptr;

	UStaticMeshComponent* Mesh = nullptr;

	UParticleSystemComponent* checkpointParticles = nullptr;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnOverlapOuter(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnOverlapEndOuter(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		int checkpointIndex = -1;
};
