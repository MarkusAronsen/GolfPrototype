// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ShapeComponent.h"
#include "UnrealMathUtility.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlinkoBlocker.generated.h"

class AGolfBall;

UCLASS()
class GOLF04_API APlinkoBlocker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlinkoBlocker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UShapeComponent* CollisionBox = nullptr;
	UStaticMeshComponent* Mesh = nullptr;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);
	
	bool ascend = false;

	float initialLocationZ;
	float ascendValue = 0.f;

	void reset();
};
