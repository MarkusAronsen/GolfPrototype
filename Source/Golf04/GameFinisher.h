// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ShapeComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFinisher.generated.h"

class AGolfBall;
class AGolfSaveGame;

UCLASS()
class GOLF04_API AGameFinisher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameFinisher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UShapeComponent* CollisionBox = nullptr;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	float timeToPlayOutro = 0.f;
	bool startFade = false;
};
