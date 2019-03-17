// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <ctime>

#include "Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SecretLevelManager.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacmanPathNode.generated.h"

class AGolfBall;
class APacmanGhost;

UCLASS()
class GOLF04_API APacmanPathNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APacmanPathNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, Category = "Pacman variable")
		bool up = false;

	UPROPERTY(EditAnywhere, Category = "Pacman variable")
		bool down = false;

	UPROPERTY(EditAnywhere, Category = "Pacman variable")
		bool left = false;

	UPROPERTY(EditAnywhere, Category = "Pacman variable")
		bool right = false;

	UShapeComponent* CollisionBox = nullptr;

	TArray<AActor*> secretLevelManager;

	void drawDebugObjects();
};
