// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Golfball.h"
#include "Components/ShapeComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructableBlock.generated.h"

UCLASS()
class GOLF04_API ADestructableBlock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructableBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UShapeComponent * CollisionBox = nullptr;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Point")
		float hitPoints = 6000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Point")
		float maxHitPoints;

	FVector startPos;

	bool bDead = false;
	float deadTimer = 0.f;

	void resetFunction();

};
