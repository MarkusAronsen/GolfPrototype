// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ctime>

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingGolfBall.generated.h"

UCLASS()
class GOLF04_API AMovingGolfBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingGolfBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* mesh = nullptr;

	float launchTimer = 0.f;


	UPROPERTY(EditAnywhere)
		float launchInterval = 4.f;

	UPROPERTY(EditAnywhere)
		float launchForce = 10000.f;

};
