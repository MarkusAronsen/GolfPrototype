// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfSaveGame.h"

UGolfSaveGame::UGolfSaveGame()
{
	slotName = TEXT("SaveSlot");
	userIndex = 0;

	if (GEngine->GetWorld() && GEngine->GetWorld()->GetGameInstance())
	{
		UGolfGameInstance* GameInstance = Cast<UGolfGameInstance>(GEngine->GetWorld()->GetGameInstance());

		for (int i = 0; i < NUM_LEVELS; i++)
			levelData[i].levelName = GameInstance->levelNames[i];
	}

	/*levelData[0].levelName = TEXT("Golf01");
	levelData[1].levelName = TEXT("Golf02");
	levelData[2].levelName = TEXT("Golf03");*/
	
	UE_LOG(LogTemp, Warning, TEXT("Save game initialized"));
	//UE_LOG(LogTemp, Warning, TEXT("Save game initialized, allocated level data is %i, num_levels * sizeof(FLevelData) is %i"), levelData.GetAllocatedSize(), NUM_LEVELS * sizeof(FLevelData));
}