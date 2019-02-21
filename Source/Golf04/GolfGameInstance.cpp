// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfGameInstance.h"
#include "GolfBall.h"

UGolfGameInstance::UGolfGameInstance(const FObjectInitializer & ObjectInitializer)	 : Super(ObjectInitializer)
{
	//if (!PowerBarWidget_BP)
		//UE_LOG(LogTemp, Warning, TEXT("No awflnawfnwa"));

	levelNames.Add(TEXT("Golf01"));
	levelNames.Add(TEXT("Golf02"));
	levelNames.Add(TEXT("Golf03"));

}

void UGolfGameInstance::initializeWidgets()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetWorld() failed"));
	}
	if (!GetWorld()->GetFirstPlayerController())
	{
		UE_LOG(LogTemp, Warning, TEXT("GetFirstPlayerController() failed"));
	}
	if (!Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetPlayerPawn() failed"));
	}
	/*if (!PlayerPowerBarWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Power bar widget not initialized"));
	}*/
}
