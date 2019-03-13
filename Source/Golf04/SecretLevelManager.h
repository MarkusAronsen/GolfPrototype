// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BowlingPin.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecretLevelManager.generated.h"

class AGolfBall;

enum SecretLevelState
{
	BOWLING = 0,
	PLINKO = 1,
	HANGMAN = 3
};


UCLASS()
class GOLF04_API ASecretLevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretLevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int secretState = -1;

	//Bowling-----------------------
	TArray<AActor*> bowlingPins;

	int bowlingThrows = 0;

	void incrementBowlingThrow();
	void removeFallenPins();
	void bowlingFinished();
	int getBowlingScore();

	float ballThrownTimer = 0.f;
	bool bBallIsThrown = false;
	bool bAllowedToRespawn = false;
	bool bStartRespawn = false;
	float respawnTimer = 0.f;

	int numPins = 10;

	int bowlingScore = 0;
	//-----------------------------

	//Plinko-----------------------

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float plinkoLaunchPower = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float plinkoMaxLaunchPower = 30000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* PlinkoPowerBarWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> PlinkoPowerBarWidget_BP;

	bool plinkoLaunchReady = true;
	bool incrementPlinkoPower = false;
	void startChargingPlinko();
	void plinkoLaunch();

	int plinkoScore = 0;

	void registerPlinkoScore(int value);

	int plinkoAttempts = 0;

	void plinkoFinished();

	//-------------- Hangman ------------------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* HangmanWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> HangmanWidget_BP;


};

