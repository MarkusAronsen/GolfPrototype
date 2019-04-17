// Fill out your copyright notice in the Description page of Project Settings.

#include "KillVolume.h"
#include "GolfBall.h"

// Sets default values
AKillVolume::AKillVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKillVolume::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = FindComponentByClass<UBoxComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Kill volume no collision box"));
}

// Called every frame
void AKillVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillVolume::OnOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		AGolfBall* playerPtr = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0));
		
		if (UGameplayStatics::GetCurrentLevelName(this).Compare("SecretLevel05", ESearchCase::IgnoreCase) == 0)
		{
			playerPtr->secretLevelManagerInstance->incrementRunnerTimer = false;
			playerPtr->secretLevelManagerInstance->runnerScore = playerPtr->secretLevelManagerInstance->runnerTimer;
			playerPtr->secretLevelManagerInstance->secretLevelFinished();
		}


		if (playerPtr->state == playerPtr->states::GOLF)
			playerPtr->strokeCounter++;
		else if (playerPtr->state == playerPtr->states::WALKING)
			playerPtr->walkingRestarts++;
		//else if(playerPtr->state == playerPtr->states::CLIMBING)
			
		//TODO:: fix different kill cases

		playerPtr->respawnAtCheckpoint();
	}
}

