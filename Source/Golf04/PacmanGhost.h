// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ShapeComponent.h"

#include "PacmanPathNode.h"
#include "SecretLevelManager.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacmanGhost.generated.h"

class AGolfBall;

namespace GBH {

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


	UShapeComponent* CollisionBox = nullptr;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere)
		float timeToActivate;

	TArray<GBH::directions> legalDirections;

	UPROPERTY(EditAnywhere)
	int directionBuffer = -1;

	UPROPERTY(EditAnywhere)
	FVector direction;

	APacmanPathNode* pathNode = nullptr;
	ASecretLevelManager* secretLevelManagerInstance = nullptr;
	bool activated = false;

	float activateTimer = 0.f;

	UPROPERTY(EditAnywhere)
		FString name = TEXT("No name");


	UFUNCTION(BlueprintImplementableEvent)
		void announceGhost(const FString &ghostName);

	bool playerIsHit = false;

	//Reset
	FVector initialPosition;
	FVector initialDirection;

	UFUNCTION(BlueprintCallable)
	void resetGhosts();
};
