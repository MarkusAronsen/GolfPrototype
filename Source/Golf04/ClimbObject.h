// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/World.h"
#include "Components/ShapeComponent.h"
#include "DrawDebugHelpers.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClimbObject.generated.h"

UCLASS()
class GOLF04_API AClimbObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClimbObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Edge node")
		bool bIsEdgeNode = false;

	UShapeComponent* CollisionBox = nullptr;

	bool ignored = false;
	float ignoreTimer = 0.f;
};
