// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define PI 3.14159265//358//9793238462643383279502884

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TranslatingGeometry2.generated.h"

UCLASS()
class GOLF04_API ATranslatingGeometry2 : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATranslatingGeometry2();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float position = PI;

};
