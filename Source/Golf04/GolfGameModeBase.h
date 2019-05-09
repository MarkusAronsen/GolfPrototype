// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WallLaser.h"
#include <Vector>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Core/Public/Math/Color.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GolfGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GOLF04_API AGolfGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGolfGameModeBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//TArray<AActor*> laserGrid;
	//float gridTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangman variable")
		FString UsedLetters = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangman variable")
		TArray <FString> targetWords;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangman variable")
		TArray <FString> targetRoles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangman variable")
		FString targetWord = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangman variable")
		int triesLeft = 16;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangman variable")
		int count = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hangman variable")
		bool gameWon = false;

};