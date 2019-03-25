// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectFloater.generated.h"

UCLASS()
class GOLF04_API AObjectFloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectFloater();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float offset = 0.f;

	UPROPERTY(EditAnywhere)
		float Xoffset = 1.f;

	UPROPERTY(EditAnywhere)
		float Yoffset = 1.f;

	UPROPERTY(EditAnywhere)
		float Zoffset = 1.f;



	UPROPERTY(EditAnywhere)
		float floatSpeed = 1.f;
};
