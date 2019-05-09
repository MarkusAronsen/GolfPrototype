// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "LevelSelecter.h"

//#include "Blueprint/UserWidget.h"
//#include "Engine/Classes/Kismet/GameplayStatics.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GolfGameInstance.generated.h"

class AGolfBall;
UCLASS()
class GOLF04_API UGolfGameInstance : public UGameInstance
{
	GENERATED_BODY()

		UGolfGameInstance(const FObjectInitializer &ObjectInitializer);
	
public:
	
	//UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "Game Instance Data")
		//TArray<FString> levelNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float cameraSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString secretLevelParentName = TEXT("no name");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool exitingSecretLevel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector secretLevelEntrancePosition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int gameInstanceStrokeCounter = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float musicVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float soundEffectVolume = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int previousState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int currentSong = 0;
};
