// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfGameInstance.h"
#include "GolfBall.h"

UGolfGameInstance::UGolfGameInstance(const FObjectInitializer & ObjectInitializer)	 : Super(ObjectInitializer)
{
	levelNames.Add(TEXT("Golf01"));
	levelNames.Add(TEXT("Golf02"));
	levelNames.Add(TEXT("Golf03"));
}


