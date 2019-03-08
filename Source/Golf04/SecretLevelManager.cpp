// Fill out your copyright notice in the Description page of Project Settings.

#include "SecretLevelManager.h"
#include "GolfBall.h"

// Sets default values
ASecretLevelManager::ASecretLevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASecretLevelManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel01"), ESearchCase::IgnoreCase) == 0)
	{
		secretState = BOWLING;
		UGameplayStatics::GetAllActorsOfClass(this, ABowlingPin::StaticClass(), bowlingPins);
	}

	if (secretState == -1)
		UE_LOG(LogTemp, Warning, TEXT("no secret state was set (begin play)"));
}

// Called every frame
void ASecretLevelManager::Tick(float DeltaTime)
{

	switch (secretState)
	{
	case BOWLING:
		if (bBallIsThrown)
		{
			ballThrownTimer += DeltaTime;
			if (ballThrownTimer >= 2.f)
			{
				ballThrownTimer = 0.f;
				bBallIsThrown = false;
				bAllowedToRespawn = true;
			}
		}

		if (bAllowedToRespawn)
		{
			if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->mMesh->GetPhysicsLinearVelocity().Size() < 1.f)
			{
				bAllowedToRespawn = false;
				bStartRespawn = true;
			}
		}
		
		if (bStartRespawn)
		{
			respawnTimer += DeltaTime;
			if (respawnTimer > 2.f)
			{
				respawnTimer = 0.f;
				bStartRespawn = false;

				if (bowlingThrows == 1)
				{
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->respawnAtCheckpoint();
					removeFallenPins();
				}
				else if (bowlingThrows == 2)
				{
					removeFallenPins();
					bowlingFinished();
				}
			}
		}
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("no secret state set (tick)"));
		break;
	}
	Super::Tick(DeltaTime);
}

void ASecretLevelManager::incrementBowlingThrow()
{
	bowlingThrows++;
	ballThrownTimer = 0.f;
	bBallIsThrown = true;
}

void ASecretLevelManager::removeFallenPins()
{
	UE_LOG(LogTemp, Warning, TEXT("Removing fallen pins"));

	for (int i = 0; i < numPins; i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), (Cast<ABowlingPin>(bowlingPins[i])->GetActorUpVector() - FVector(0, 0, 1)).Size());
		if ((Cast<ABowlingPin>(bowlingPins[i])->GetActorUpVector() - FVector(0, 0, 1)).Size() > 0.2f)
		{
			bowlingPins[i]->Destroy();
		}
	}

}

void ASecretLevelManager::bowlingFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Bowling finished"));
}
