// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BilliardsHole.generated.h"

class AGolfBall;
class ABilliardBall;

UCLASS()
class GOLF04_API ABilliardsHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABilliardsHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		UShapeComponent* CollisionBox = nullptr;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	float settleTimer = 0.f;
	bool startSettleTimer = false;
	AActor* OtherBillardBall = nullptr;

	TArray<AActor*> BilliardBalls;
};
