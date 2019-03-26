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

	UPROPERTY(EditAnywhere, Category = "Translation")
		float Xoffset = 1.f;

	UPROPERTY(EditAnywhere, Category = "Translation")
		float Yoffset = 1.f;

	UPROPERTY(EditAnywhere, Category = "Translation")
		float Zoffset = 1.f;

	UPROPERTY(EditAnywhere, Category = "Translation")
		float floatSpeed = 1.f;


	UPROPERTY(EditAnywhere, Category = "Rotation")
		bool rotatePitch = false;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		bool rotateYaw = false;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		bool rotateRoll = false;

	UPROPERTY(EditAnywhere, Category = "Rotation")
		float rotateSpeed = 0.f;
};
