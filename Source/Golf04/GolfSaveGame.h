// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Golf04.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GolfSaveGame.generated.h"

/**
 * 
 */

const int NUM_LEVELS = 3;

USTRUCT()
struct FLevelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString levelName;
	
	UPROPERTY()
		int starRating;

	UPROPERTY()
		float timeElapsed;

	FLevelData()
	{
		levelName = TEXT("");
		starRating = -1;
		timeElapsed = -1;
	}

};

UCLASS()
class GOLF04_API UGolfSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UGolfSaveGame();

	UPROPERTY(VisibleAnywhere, Category = "Save data")
		FString slotName;

	UPROPERTY(VisibleAnywhere, Category = "Save data")
		int userIndex;

	UPROPERTY()
		TArray<FLevelData> levelData = { FLevelData(), FLevelData(), FLevelData() };

};
