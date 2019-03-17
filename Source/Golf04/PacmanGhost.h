// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PacmanPathNode.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacmanGhost.generated.h"

namespace golf {

	enum directions {
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3
	};
}


UCLASS()
class GOLF04_API APacmanGhost : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APacmanGhost();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
		float timeToActivate;

	TArray<golf::directions> legalDirections;

	UPROPERTY(EditAnywhere)
	int directionBuffer = -1;

	UPROPERTY(EditAnywhere)
	FVector direction;

	APacmanPathNode* pathNode = nullptr;
};
