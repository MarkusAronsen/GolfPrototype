// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameEngine.h"
#include "Golf04.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GolfSaveGame.generated.h"

//class AGolfSaveInstance;
const int NUM_LEVELS = 10;

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		FString levelName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int starRating;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		float timeElapsed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int currentCheckpoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
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


	//Cool and smart and sensible code
	UPROPERTY(BlueprintReadOnly)
		TArray<FLevelData> levelData = { FLevelData(), FLevelData(), FLevelData(), 
										FLevelData(), FLevelData(), FLevelData(), 
										FLevelData(), FLevelData(), FLevelData(),
										FLevelData() };
};
