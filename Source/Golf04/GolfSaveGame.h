// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameEngine.h"
#include "Golf04.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GolfSaveGame.generated.h"

//class AGolfSaveInstance;
const int NUM_LEVELS = 11;

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
		int secretLevelPerformance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int currentCheckpoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		bool bLevelCompleted;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int flyingRestarts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int climbingRestarts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int walkingRestarts;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int golfStrokes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		TArray<int> golfPar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		TArray<int> walkingPar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		TArray<int> climbingPar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		TArray<int> flyingPar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		int secretPersonalBest;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		bool golfDialoguePlayed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		bool walkingDialoguePlayed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		bool climbDialoguePlayed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level data")
		bool flyingDialoguePlayed;

	FLevelData()
	{
		levelName = TEXT("");
		starRating = -1;
		flyingRestarts = -1;
		climbingRestarts = -1;
		walkingRestarts = -1;
		golfStrokes = -1;
		timeElapsed = -1;
		secretLevelPerformance = -1;
		currentCheckpoint = -1;
		bLevelCompleted = false;
		golfPar = {};
		walkingPar = {};
		climbingPar = {};
		flyingPar = {};
		golfDialoguePlayed = false;
		walkingDialoguePlayed = false;
		climbDialoguePlayed = false;
		flyingDialoguePlayed = false;

		secretPersonalBest = -1;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool gameCompleted = false;

	//Cool and smart and sensible code
	UPROPERTY(BlueprintReadOnly)
		TArray<FLevelData> levelData = { FLevelData(), FLevelData(), FLevelData(), 
										FLevelData(), FLevelData(), FLevelData(), 
										FLevelData(), FLevelData(), FLevelData(),
										FLevelData(), FLevelData(), };
};
