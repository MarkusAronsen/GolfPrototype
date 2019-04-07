// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ShapeComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveablePlatform.generated.h"

class AGolfBall;

UCLASS()
class GOLF04_API AMoveablePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveablePlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	AGolfBall* playerPtr = nullptr;

	float moveSpeed = 300.f;

	UShapeComponent* CollisionBox = nullptr;

	UStaticMeshComponent* Up = nullptr;
	UStaticMeshComponent* Down = nullptr;
	UStaticMeshComponent* Left = nullptr;
	UStaticMeshComponent* Right = nullptr;

	UStaticMeshComponent* Pad = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool upPushed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool downPushed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool leftPushed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool rightPushed = false;

	bool onMoveablePlatform = false;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);


	UFUNCTION()
		void OnOverlapUp(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlapUp(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapDown(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlapDown(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapLeft(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlapLeft(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapRight(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnEndOverlapRight(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex);
};
