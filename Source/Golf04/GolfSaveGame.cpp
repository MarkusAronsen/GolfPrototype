// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfSaveGame.h"

//#include "GolfGameInstance.h"


UGolfSaveGame::UGolfSaveGame()
{
	slotName = TEXT("SaveSlot");
	userIndex = 0;

	levelData[0].levelName = TEXT("TestLevel01");
	levelData[1].levelName = TEXT("TestLevel02");
	levelData[2].levelName = TEXT("TestLevel03");
	levelData[3].levelName = TEXT("TestLevel04");
	levelData[4].levelName = TEXT("TestLevel05");

	//Secret levels
	levelData[5].levelName = TEXT("SecretLevel01");
	levelData[6].levelName = TEXT("SecretLevel02");
	levelData[7].levelName = TEXT("SecretLevel03");
	levelData[8].levelName = TEXT("SecretLevel04");

	UE_LOG(LogTemp, Warning, TEXT("Save game initialized"));
	//UE_LOG(LogTemp, Warning, TEXT("Save game initialized, allocated level data is %i, num_levels * sizeof(FLevelData) is %i"), levelData.GetAllocatedSize(), NUM_LEVELS * sizeof(FLevelData)
}
