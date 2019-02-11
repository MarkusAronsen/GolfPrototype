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

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
		FString levelName;
	
	UPROPERTY(BlueprintReadOnly)
		int starRating;

	UPROPERTY(BlueprintReadOnly)
		float timeElapsed;

	UPROPERTY(BlueprintReadOnly)
		int currentCheckpoint;

	UPROPERTY(BlueprintReadOnly)
		bool bLevelCompleted;

	FLevelData()
	{
		levelName = TEXT("");
		starRating = -1;
		timeElapsed = -1;
		currentCheckpoint = -1;
		bLevelCompleted = false;
	}
};

UCLASS()
class GOLF04_API UGolfSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UGolfSaveGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save data")
		FString slotName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Save data")
		int userIndex;

	UPROPERTY(BlueprintReadOnly)
		TArray<FLevelData> levelData = { FLevelData(), FLevelData(), FLevelData() };

};
