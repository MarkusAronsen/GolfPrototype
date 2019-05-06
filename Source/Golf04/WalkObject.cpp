// Fill out your copyright notice in the Description page of Project Settings.

#include "WalkObject.h"
#include "GolfSaveGame.h"
#include "GolfBall.h"

// Sets default values
AWalkObject::AWalkObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"), true);
	CollisionBox->SetBoxExtent(FVector(20, 10, 20));
}

// Called when the game starts or when spawned
void AWalkObject::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWalkObject::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Golf Object no collision box"));
	
}

// Called every frame
void AWalkObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWalkObject::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()) && static_cast<AGolfBall*>(OtherActor)->state != static_cast<AGolfBall*>(OtherActor)->states::WALKING)
	{
		static_cast<AGolfBall*>(OtherActor)->state = static_cast<AGolfBall*>(OtherActor)->states::WALKING;
		static_cast<AGolfBall*>(OtherActor)->golfInit();

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
			if (!LoadGameInstance->levelData[levelIndex].walkingDialoguePlayed)
			{
				SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
				SaveGameInstance->levelData[levelIndex].walkingDialoguePlayed = true;
				UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
				Cast<AGolfBall>(OtherActor)->displayDialogue();
			}
		}
	}
}
