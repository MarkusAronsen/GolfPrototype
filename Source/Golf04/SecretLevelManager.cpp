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
		UE_LOG(LogTemp, Warning, TEXT("Found %i pins"), bowlingPins.Num());
	}
	else if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel02"), ESearchCase::IgnoreCase) == 0)
	{
		secretState = PLINKO;
		if (PlinkoPowerBarWidget_BP)
		{
			PlinkoPowerBarWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), PlinkoPowerBarWidget_BP);
			if (PlinkoPowerBarWidget)
			{
				PlinkoPowerBarWidget->AddToViewport();
				PlinkoPowerBarWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("PlinkoPowerBarWidget not initialized"));

	}
	else if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel03"), ESearchCase::IgnoreCase) == 0)
	{
		secretState = BILLIARDS;
	}


	if (secretState == -1)
		UE_LOG(LogTemp, Warning, TEXT("no secret state was set (begin play)"));
}

// Called every frame
void ASecretLevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
					if (bowlingScore == 10)
						bowlingFinished();
				}
				else if (bowlingThrows == 2)
				{
					removeFallenPins();
					bowlingFinished();
					UE_LOG(LogTemp, Warning, TEXT("Bowling score: %i"), getBowlingScore());
				}
			}
		}
		break;

	case PLINKO:

		if(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state != Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::PLINKO)
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::PLINKO;

		if (incrementPlinkoPower)
		{
			if(plinkoLaunchPower <= plinkoMaxLaunchPower)
				plinkoLaunchPower += 15000 * DeltaTime;
		}

		break;

	case BILLIARDS:

		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("no secret state set (tick)"));
		break;
	}
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

	for (int i = numPins - 1; i >= 0; i--)
	{
		if ((Cast<ABowlingPin>(bowlingPins[i])->GetActorUpVector() - FVector(0, 0, 1)).Size() > 0.2f)
		{
			bowlingPins[i]->Destroy();
			bowlingPins.RemoveAt(i, 1);
			numPins--;
			bowlingScore++;
		}
	}

}

void ASecretLevelManager::bowlingFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Bowling finished"));

	//TODO: give player return to level hud? return player to level on timer? display secret level score?
}

int ASecretLevelManager::getBowlingScore()
{
	return bowlingScore;
}

void ASecretLevelManager::registerPlinkoScore(int value)
{
	plinkoScore += value;
	plinkoAttempts++;
	Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->respawnAtCheckpoint();
	plinkoLaunchReady = true;

	UE_LOG(LogTemp, Warning, TEXT("Plinko score: %i, attempt %i"), plinkoScore, plinkoAttempts);
	if (plinkoScore == 3)
		plinkoFinished();
}

void ASecretLevelManager::startChargingPlinko()
{
	plinkoLaunchReady = false;
	incrementPlinkoPower = true;

	PlinkoPowerBarWidget->SetVisibility(ESlateVisibility::Visible);
}

void ASecretLevelManager::plinkoLaunch()
{
	Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->mMesh->AddImpulse(FVector(0.f, 0.f, plinkoLaunchPower), NAME_None, true);
	plinkoLaunchPower = 0.f;
	incrementPlinkoPower = false;

	PlinkoPowerBarWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ASecretLevelManager::plinkoFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("Plinko finished"));

	//TODO: give player return to level hud? return player to level on timer? display secret level score?
}