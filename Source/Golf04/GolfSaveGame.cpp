// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfSaveGame.h"

//#include "GolfGameInstance.h"


UGolfSaveGame::UGolfSaveGame()
{
	slotName = TEXT("SaveSlot");
	userIndex = 0;

	//Main levels
	levelData[0].levelName = TEXT("Level01");
	levelData[0].golfPar.Add(1);
	levelData[0].golfPar.Add(2);
	levelData[0].golfPar.Add(3);

	levelData[1].levelName = TEXT("Level02");
	levelData[1].golfPar.Add(2);
	levelData[1].golfPar.Add(3);
	levelData[1].golfPar.Add(4);

	levelData[2].levelName = TEXT("Level03");
	levelData[2].golfPar.Add(4);
	levelData[2].golfPar.Add(6);
	levelData[2].golfPar.Add(7);

	levelData[3].levelName = TEXT("Level04");
	levelData[3].golfPar.Add(4);
	levelData[3].golfPar.Add(6);
	levelData[3].golfPar.Add(8);

	levelData[4].levelName = TEXT("Level05");
	levelData[4].golfPar.Add(10);
	levelData[4].golfPar.Add(11);
	levelData[4].golfPar.Add(12);

	levelData[5].levelName = TEXT("Level06");
	levelData[5].golfPar.Add(10);
	levelData[5].golfPar.Add(11);
	levelData[5].golfPar.Add(12);

	levelData[6].levelName = TEXT("Level07");
	levelData[6].golfPar.Add(10);
	levelData[6].golfPar.Add(11);
	levelData[6].golfPar.Add(12);

	//Secret levels
	levelData[7].levelName = TEXT("SecretLevel01");
	levelData[8].levelName = TEXT("SecretLevel02");
	levelData[9].levelName = TEXT("SecretLevel04");
	levelData[10].levelName = TEXT("SecretLevel05");

	//Work in progress
	/*levelData[11].levelName = TEXT("FlyingLevel01");
	levelData[12].levelName = TEXT("TestWalking02");
	levelData[13].levelName = TEXT("ClimbingTest02");
	levelData[14].levelName = TEXT("TestWalking03");
	*/
	
	UE_LOG(LogTemp, Warning, TEXT("Save game initialized"));
	//UE_LOG(LogTemp, Warning, TEXT("Save game initialized, allocated level data is %i, num_levels * sizeof(FLevelData) is %i"), levelData.GetAllocatedSize(), NUM_LEVELS * sizeof(FLevelData)
}
