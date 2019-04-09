// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyingObstacle.generated.h"

class AGolfBall;

UCLASS()
class GOLF04_API AFlyingObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlyingObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* CollisionBox = nullptr;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);


	UPROPERTY(EditAnywhere, Category = "Translation")
		bool translateX = false;

	UPROPERTY(EditAnywhere, Category = "Translation")
		bool translateZ = false;

	UPROPERTY(EditAnywhere, Category = "Translation")
		float translateRange = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Translation")
	float translateOffset = 0.f;

	UPROPERTY(EditAnywhere, Category = "Translation")
		float translateSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		bool rotatePitch = false;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float rotateSpeed = 0.f;


	void resetFlyingObstacle();

	FVector initialPosition;
	FRotator initialRotation;

	bool startResetTimer = false;

	float resetTimer = 0.f;
};
