// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfSaveGame.h"

#include "GolfGameInstance.h"

UGolfSaveGame::UGolfSaveGame()
{
	slotName = TEXT("SaveSlot");
	userIndex = 0;

	UE_LOG(LogTemp, Warning, TEXT("Save game initialized"));
	//UE_LOG(LogTemp, Warning, TEXT("Save game initialized, allocated level data is %i, num_levels * sizeof(FLevelData) is %i"), levelData.GetAllocatedSize(), NUM_LEVELS * sizeof(FLevelData)
}
void UGolfSaveGame::initLevelNames()
{
	if (GEngine && GEngine->GetWorld() && GEngine->GetWorld()->GetGameInstance())
	{
		UE_LOG(LogTemp, Warning, TEXT("HEISANNSVEISANN"));
		UGolfGameInstance* GameInstance = Cast<UGolfGameInstance>(GEngine->GetWorld()->GetGameInstance());
		for (int i = 0; i < NUM_LEVELS; i++)
			levelData[i].levelName = GameInstance->levelNames[i];
	}
}