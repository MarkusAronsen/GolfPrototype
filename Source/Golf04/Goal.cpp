// Fill out your copyright notice in the Description page of Project Settings.

#include "Goal.h"
#include "GolfBall.h"


// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<USphereComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Goal no collision box"));

	}
	
	levelName = UGameplayStatics::GetCurrentLevelName(this);
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGoal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		levelTimeElapsed = UGameplayStatics::GetUnpausedTimeSeconds(this);
		saveLevelData();
		UGameplayStatics::OpenLevel(this, "LevelSelect");
	}
}

void AGoal::saveLevelData()
{
	UGolfSaveGame* SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));
	UGolfSaveGame* LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));

	if (!UGameplayStatics::DoesSaveGameExist(SaveGameInstance->slotName, SaveGameInstance->userIndex))
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);

	LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->slotName, LoadGameInstance->userIndex));

	int levelIndex = -1;

	for (int i = 0; i < NUM_LEVELS; i++)
	{
		if (SaveGameInstance->levelData[i].levelName.Compare(levelName, ESearchCase::CaseSensitive) == 0)
		{
			levelIndex = i;
		}
	}

	if (levelIndex != -1)
	{
		if (LoadGameInstance->levelData[levelIndex].timeElapsed > levelTimeElapsed || LoadGameInstance->levelData[levelIndex].timeElapsed < 0)
		{
			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].timeElapsed = levelTimeElapsed;
			SaveGameInstance->levelData[levelIndex].currentCheckpoint = -1;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
		}

			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].currentCheckpoint = -1;
			SaveGameInstance->levelData[levelIndex].bLevelCompleted = true;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Invalid level index"));
}
