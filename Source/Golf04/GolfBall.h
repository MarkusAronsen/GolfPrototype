// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Goal.h"
#include "Components/SphereComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GolfBall.generated.h"

UCLASS()
class GOLF04_API AGolfBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGolfBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		UShapeComponent * CollisionBox = nullptr;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelVariable")
		int strokeCounter = 0;


	
};
