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

	UE_LOG(LogTemp, Warning, TEXT("Save game initialized"));
	//UE_LOG(LogTemp, Warning, TEXT("Save game initialized, allocated level data is %i, num_levels * sizeof(FLevelData) is %i"), levelData.GetAllocatedSize(), NUM_LEVELS * sizeof(FLevelData)
}
/*void UGolfSaveGame::initLevelNames()
{
	if (GEngine && GEngine->GetWorld() && GEngine->GetWorld()->GetGameInstance())
	{
		UE_LOG(LogTemp, Warning, TEXT("HEISANNSVEISANN"));
		UGolfGameInstance* GameInstance = Cast<UGolfGameInstance>(GEngine->GetWorld()->GetGameInstance());
		for (int i = 0; i < NUM_LEVELS; i++)
			levelData[i].levelName = GameInstance->levelNames[i];
	}
}*/