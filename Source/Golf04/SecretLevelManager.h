// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MallocLeakDetection.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "BowlingPin.h"
#include "RunnerFloor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecretLevelManager.generated.h"

class AGolfBall;
class APacmanPathNode;

enum SecretLevelState
{
	BOWLING = 0,
	PLINKO = 1,
	BILLIARDS = 2,
	PACMAN = 3,
	RUNNER = 4
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* SecretLevelFinishedWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		TSubclassOf<class UUserWidget> SecretLevelFinishedWidget_BP;

	FString secretLevelParent;

	//Bowling---------------------------------------------------------
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int bowlingScore = 0;
	//----------------------------------------------------------------


	//Plinko----------------------------------------------------------
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int plinkoScore = 0;

	void registerPlinkoScore(int value);

	int plinkoAttempts = 0;

	void plinkoFinished();

	//----------------------------------------------------------------	
	
	//Billiards-------------------------------------------------------
	void registerBilliards();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int billiardsScore = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int billiardsShotsUsed = 0;

	void billiardsFinished(bool lostTo8Ball);

	int getBilliardsScore();
	//----------------------------------------------------------------

	//Pacman----------------------------------------------------------
	FVector buffer;
	bool overlappingNode = false;
	APacmanPathNode* pacmanNode = nullptr;
	bool gameStarted = false;
	bool walkForward = false;

	float pacmanMoveSpeed = 400;

	void pacmanSwitchDirection();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int pacmanScore = 0;

	int getPacmanScore();

	void pacmanFinished();
	//----------------------------------------------------------------

	//Runner----------------------------------------------------------
	int objectsSpawned = 0;
	float runnerMoveSpeed = 200.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float runnerScore = 0;

	float getRunnerScore();

	void runnerFinished();
	//----------------------------------------------------------------
};

