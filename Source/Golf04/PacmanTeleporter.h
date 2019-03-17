// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ShapeComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PacmanTeleporter.generated.h"

class AGolfBall;
class APacmanGhost;

UCLASS()
class GOLF04_API APacmanTeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APacmanTeleporter();

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
		bool leftTeleporter = false;

	UPROPERTY(EditAnywhere)
		bool rightTeleporter = false;

};
