// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFinisher.h"
#include "GolfBall.h"
#include "GolfSaveGame.h"

// Sets default values
AGameFinisher::AGameFinisher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameFinisher::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = FindComponentByClass<UShapeComponent>();
	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGameFinisher::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Game finisher no collision box"));
}

// Called every frame
void AGameFinisher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (startFade)
	{
		timeToPlayOutro += DeltaTime;
		if (timeToPlayOutro >= 6.f)
		{
			timeToPlayOutro = 0.f;
			startFade = false;
			UGameplayStatics::OpenLevel(this, "Outro");
		}
	}
}

void AGameFinisher::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		GetWorld()->GetFirstPlayerController()->ClientSetCameraFade(true, FColor::White, FVector2D(0.f, 1.f), 6.2f, true);
		startFade = true;

		UGolfSaveGame* SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));
		UGolfSaveGame* LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));

		if (!UGameplayStatics::DoesSaveGameExist(SaveGameInstance->slotName, SaveGameInstance->userIndex))
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);

		LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->slotName, LoadGameInstance->userIndex));

		int levelIndex = -1;

		for (int i = 0; i < NUM_LEVELS; i++)
		{
			if (SaveGameInstance->levelData[i].levelName.Compare(UGameplayStatics::GetCurrentLevelName(this), ESearchCase::IgnoreCase) == 0)
			{
				levelIndex = i;
			}
		}

		if (levelIndex != -1)
		{
			if (LoadGameInstance->levelData[levelIndex].golfStrokes > Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->strokeCounter
				|| LoadGameInstance->levelData[levelIndex].golfStrokes < 0)
			{
				SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
				SaveGameInstance->levelData[levelIndex].golfStrokes = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->strokeCounter;
				UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
			}
			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].currentCheckpoint = -1;
			SaveGameInstance->levelData[levelIndex].bLevelCompleted = true;
			SaveGameInstance->gameCompleted = true;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Invalid level index"));
	}
}

