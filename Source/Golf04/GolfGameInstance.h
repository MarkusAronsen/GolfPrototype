// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "LevelSelecter.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GolfGameInstance.generated.h"

class AGolfBall;
/**
 * 
 */
UCLASS()
class GOLF04_API UGolfGameInstance : public UGameInstance
{
	GENERATED_BODY()

		UGolfGameInstance(const FObjectInitializer &ObjectInitializer);
	
public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game instance data")
	TArray<FString> levelNames;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")	
		//TSubclassOf<class UUserWidget> PowerBarWidget_BP;


	//UUserWidget* PlayerPowerBarWidget = nullptr;

	void initializeWidgets();

	//bool bWidgetsInitialized = false;
};
