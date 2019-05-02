// Fill out your copyright notice in the Description page of Project Settings.

#include "SecretLevelManager.h"
#include "GolfBall.h"
#include "PacmanPathNode.h"
#include "PacmanGhost.h"
#include "PacmanPellet.h"
#include "BowlingPin.h"
#include "RunnerFloor.h"
//#include "PhysicsMaze.h"
#include "PhysicsMazeRotator.h"
#include "GolfSaveGame.h"

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
	else if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel04"), ESearchCase::IgnoreCase) == 0)
	{
		secretState = PACMAN;
		buffer = FVector::ZeroVector;
		UGameplayStatics::GetAllActorsOfClass(this, APacmanPellet::StaticClass(), pellets);
		setPacmanAntiAliasing();
	}
	else if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel05"), ESearchCase::IgnoreCase) == 0)
	{
		secretState = RUNNER;
		incrementRunnerTimer = true;
	}
	/*else if (UGameplayStatics::GetCurrentLevelName(this).Compare(TEXT("SecretLevel06"), ESearchCase::IgnoreCase) == 0)
	{
		secretState = MAZE;

		TArray<AActor*> mazeArr;

		UGameplayStatics::GetAllActorsOfClass(this, APhysicsMaze::StaticClass(), mazeArr);

		if (mazeArr.Num() == 1)
			mazePtr = Cast<APhysicsMaze>(mazeArr[0]);
		else
			UE_LOG(LogTemp, Warning, TEXT("Maze not initialized"));

		UGameplayStatics::GetAllActorsOfClass(this, APhysicsMazeRotator::StaticClass(), mazeArr);
		if (mazeArr.Num() == 2)
		{
			mazeRotator1 = Cast<APhysicsMazeRotator>(mazeArr[0]);
			mazeRotator2 = Cast<APhysicsMazeRotator>(mazeArr[1]);
		}
			
		else
			UE_LOG(LogTemp, Warning, TEXT("Maze rotators not initialized"));
	}*/

	if (secretState == -1)
		UE_LOG(LogTemp, Warning, TEXT("no secret state was set (begin play)"));
}

// Called every frame
void ASecretLevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector playerLocation = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorLocation();

	switch (secretState)
	{
	case BOWLING:
		if (bBallIsThrown && Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->mMesh->GetPhysicsLinearVelocity().Size() < 1.5f && !Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->bRespawning)
		{
			ballThrownTimer += DeltaTime;
			if (ballThrownTimer >= 0.5f)
			{
				ballThrownTimer = 0.f;
				//bBallIsThrown = false;
				//bAllowedToRespawn = true;
				bStartRespawn = true;
			}
		}

		/*if (bAllowedToRespawn)
		{
			if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->mMesh->GetPhysicsLinearVelocity().Size() < 1.f)
			{
				bAllowedToRespawn = false;
				bStartRespawn = true;
			}
		}*/
		
		if (bStartRespawn)
		{
			//respawnTimer += DeltaTime;
			//if (respawnTimer > 2.f)
			//{
				//respawnTimer = 0.f;
				
			bStartRespawn = false;

				if (bowlingThrows == 1)
				{
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->respawnAtCheckpoint();
					removeFallenPins();
					if (bowlingScore == 10)
						//bowlingFinished();	
						secretLevelFinished();
				}
				else if (bowlingThrows == 2)
				{
					removeFallenPins();
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->respawnAtCheckpoint();
					//bowlingFinished();
					secretLevelFinished();
					UE_LOG(LogTemp, Warning, TEXT("Bowling score: %i"), getBowlingScore());
				}
			//}
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

	case PACMAN:
		if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state != Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::PACMAN)
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::PACMAN;

		//Has player started moving?
		if(gameStarted)
		{ 
			//Is player currently at node?
			if (pacmanNode && (pacmanNode->GetActorLocation() - playerLocation).Size() < 20.f)
			{
				//Is player attemting to move in legal direction at node? If yes, set player rotation and consume buffer, set player to move forward
				if (pacmanNode->up && buffer.X > 0.5f)
				{
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(1.f, 0.f, 0.f).Rotation());
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(pacmanNode->GetActorLocation());
					buffer = FVector::ZeroVector;
					walkForward = true;
				}
				else if (pacmanNode->down && buffer.X < -0.5f)
				{
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(-1.f, 0.f, 0.f).Rotation());
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(pacmanNode->GetActorLocation());
					buffer = FVector::ZeroVector;
					walkForward = true;
				}
				else if (pacmanNode->left && buffer.Y < -0.5f)
				{
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(0.f, -1.f, 0.f).Rotation());
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(pacmanNode->GetActorLocation());
					buffer = FVector::ZeroVector;
					walkForward = true;
				}
				else if (pacmanNode->right && buffer.Y > 0.5f)
				{
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(0.f, 1.f, 0.f).Rotation());
					Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(pacmanNode->GetActorLocation());
					buffer = FVector::ZeroVector;
					walkForward = true;
				}

				//Is player attempting to move in an illegal direction at node?
				else if ((buffer.Y > 0.5 && !pacmanNode->right) || (buffer.Y < -0.5 && !pacmanNode->left) || (buffer.X > 0.5 && !pacmanNode->up) || (buffer.X < -0.5 && !pacmanNode->down))
				{
					walkForward = false;
					//Test if player should be allowed to continue moving past node
					if ((Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().X > 0.5f && pacmanNode->up) ||
						(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().X < -0.5f && pacmanNode->down) ||
						(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().Y > 0.5f && pacmanNode->right) ||
						(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().Y < -0.5f && pacmanNode->left))
					{
						if (buffer == FVector::ZeroVector)
							walkForward = false;
						else
							walkForward = true;
					}
					else
						walkForward = false;
				}				

				if ((Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().X > 0.5f && !pacmanNode->up) ||
					(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().X < -0.5f && !pacmanNode->down) ||
					(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().Y > 0.5f && !pacmanNode->right) ||
					(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().Y < -0.5f && !pacmanNode->left))
					walkForward = false;
			}


			if (walkForward)
				Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(playerLocation + Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector() * pacmanMoveSpeed * DeltaTime);


			pacmanSwitchDirection();
		}

		activateTimer += DeltaTime;

		break;

	case RUNNER:
		if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state != Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::WALKING)
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::WALKING;

		runnerMoveSpeed += DeltaTime * 7.5f;
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorLocation() + FVector(-1, 0, 0) * DeltaTime * runnerMoveSpeed);
		
		if(incrementRunnerTimer)
			runnerTimer += DeltaTime;

		break;

	case MAZE:

		//mCamera->RelativeRotation.Pitch = FMath::Clamp(mCamera->RelativeRotation.Pitch + (mouseY * cameraSpeed), -10.f, 30.f);
		/*
		if (mazeRotateW)
		{
			mazePtr->SetActorRotation(FRotator(FMath::Clamp(mazePtr->GetActorRotation().Pitch - mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue), mazePtr->GetActorRotation().Yaw, mazePtr->GetActorRotation().Roll));
			mazeRotator2->SetActorRelativeRotation(FRotator(FMath::Clamp(mazeRotator2->GetActorRotation().Pitch - mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue), mazeRotator2->GetActorRotation().Yaw, mazeRotator2->GetActorRotation().Roll));

		}

		if(mazeRotateS)
		{
			mazePtr->SetActorRotation(FRotator(FMath::Clamp(mazePtr->GetActorRotation().Pitch + mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue), mazePtr->GetActorRotation().Yaw, mazePtr->GetActorRotation().Roll));
			mazeRotator2->SetActorRelativeRotation(FRotator(FMath::Clamp(mazeRotator2->GetActorRotation().Pitch + mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue), mazeRotator2->GetActorRotation().Yaw, mazeRotator2->GetActorRotation().Roll));
		}

		if (mazeRotateA)
		{
			mazePtr->SetActorRotation(FRotator(mazePtr->GetActorRotation().Pitch, mazePtr->GetActorRotation().Yaw, FMath::Clamp(mazePtr->GetActorRotation().Roll - mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue)));
			mazeRotator1->SetActorRelativeRotation(FRotator(FMath::Clamp(mazeRotator1->GetActorRotation().Pitch + mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue), mazeRotator1->GetActorRotation().Yaw, mazeRotator1->GetActorRotation().Roll));

		}

		if (mazeRotateD)
		{
			mazePtr->SetActorRotation(FRotator(mazePtr->GetActorRotation().Pitch, mazePtr->GetActorRotation().Yaw, FMath::Clamp(mazePtr->GetActorRotation().Roll + mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue)));
			mazeRotator1->SetActorRelativeRotation(FRotator(FMath::Clamp(mazeRotator1->GetActorRotation().Pitch - mazeRotateValue, -mazeMinMaxValue, mazeMinMaxValue), mazeRotator1->GetActorRotation().Yaw, mazeRotator1->GetActorRotation().Roll));

		}
		*/
		break;


	default:
		UE_LOG(LogTemp, Warning, TEXT("no secret state set (tick)"));
		break;
	}
}

int ASecretLevelManager::getSecretLevelPerformance()
{
	FString levelName = UGameplayStatics::GetCurrentLevelName(this);

	//return bowling performance
	if (levelName.Compare(TEXT("SecretLevel01"), ESearchCase::IgnoreCase) == 0)
	{
		if (bowlingScore == bowlingPar[2])
			return 3;

		if (bowlingScore > bowlingPar[1])
			return 2;

		if (bowlingScore > bowlingPar[0])
			return 1;
	}

	//return plinko performance
	if (levelName.Compare(TEXT("SecretLevel02"), ESearchCase::IgnoreCase) == 0)
	{
		if (plinkoScore == plinkoPar[2])
			return 3;
		if (plinkoScore > plinkoPar[1])
			return 2;
		if (plinkoScore > plinkoPar[0])
			return 1;
		}

	//return billiards performance
	if (levelName.Compare(TEXT("SecretLevel03"), ESearchCase::IgnoreCase) == 0)
	{

	}

	//return pacman performance
	if (levelName.Compare(TEXT("SecretLevel04"), ESearchCase::IgnoreCase) == 0)
	{
		if (pacmanScore == pacmanPar[2])
			return 3;
		if (pacmanScore > pacmanPar[1])
			return 2;
		if (pacmanScore > pacmanPar[0])
			return 1;
	}

	//return runner performance
	if (levelName.Compare(TEXT("SecretLevel05"), ESearchCase::IgnoreCase) == 0)
	{
		if (runnerScore > runnerPar[2])
			return 3;
		if (runnerScore > runnerPar[1])
			return 2;
		if (runnerScore > runnerPar[0])
			return 1;
	}

	return 0;
}

void ASecretLevelManager::secretLevelFinished(bool lostTo8Ball)
{
	saveSecretLevelData();

	if (SecretLevelFinishedWidget_BP)
	{
		SecretLevelFinishedWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), SecretLevelFinishedWidget_BP);
		if (SecretLevelFinishedWidget)
			SecretLevelFinishedWidget->AddToViewport();
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("SecretLevelFinishedWidget not initialized"));

	UE_LOG(LogTemp, Warning, TEXT("%s is finished"), *UGameplayStatics::GetCurrentLevelName(this));
	UGameplayStatics::SetGamePaused(this, true),
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
}

void ASecretLevelManager::saveSecretLevelData()
{
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
		if (LoadGameInstance->levelData[levelIndex].secretLevelPerformance < getSecretLevelPerformance() || LoadGameInstance->levelData[levelIndex].secretLevelPerformance < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Saving secret level performance of %i"), getSecretLevelPerformance());
			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].secretLevelPerformance = getSecretLevelPerformance();
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
		}
		SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
		SaveGameInstance->levelData[levelIndex].bLevelCompleted = true;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
		/*if (LoadGameInstance->levelData[levelIndex].timeElapsed > levelTimeElapsed || LoadGameInstance->levelData[levelIndex].timeElapsed < 0)
		{
			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].timeElapsed = levelTimeElapsed;
			SaveGameInstance->levelData[levelIndex].currentCheckpoint = -1;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
		}*/

		/*SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
		SaveGameInstance->levelData[levelIndex].currentCheckpoint = -1;
		SaveGameInstance->levelData[levelIndex].bLevelCompleted = true;
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);*/
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Invalid level index"));
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

	//UE_LOG(LogTemp, Warning, TEXT("Plinko score: %i, attempt %i"), plinkoScore, plinkoAttempts);
	if (plinkoAttempts == 3)
		secretLevelFinished();
		//plinkoFinished();
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

void ASecretLevelManager::registerBilliards()
{
	billiardsScore++;
}

int ASecretLevelManager::getBilliardsScore()
{
	return billiardsScore;
}

void ASecretLevelManager::hitGhost()
{
	//pacmanLives--;
	//if (pacmanLives == 0)
	//{
	hitGhostEvent();
	walkForward = false;
	resetPacmanAntiAliasing();
	secretLevelFinished();
	//}
	//else
	//{
		//hitGhostEvent();
		//walkForward = false;
		/*Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(FVector(-1067, 0, 113));
		TArray<AActor*> ghosts;
		UGameplayStatics::GetAllActorsOfClass(this, APacmanGhost::StaticClass(), ghosts);

		for (int i = 0; i < ghosts.Num(); i++)
		{
			Cast<APacmanGhost>(ghosts[i])->reset();
		}*/
	//}
	//activateTimer = 0.f;
}

void ASecretLevelManager::pacmanSwitchDirection()
{
	if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().X > 0.5f && buffer.X < -0.5f)
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(-1, 0, 0).Rotation());
		buffer = FVector::ZeroVector;
	}
	else if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().X < -0.5f && buffer.X > 0.5f)
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(1, 0, 0).Rotation());
		buffer = FVector::ZeroVector;
	}
	else if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().Y > 0.5f && buffer.Y < -0.5f)
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(0, -1, 0).Rotation());
		buffer = FVector::ZeroVector;
	}
	else if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().Y < -0.5f && buffer.Y > 0.5f)
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(FVector(0, 1, 0).Rotation());
		buffer = FVector::ZeroVector;
	}
}

int ASecretLevelManager::getPacmanScore()
{
	return pacmanScore;
}

float ASecretLevelManager::getRunnerScore()
{
	return runnerScore;
}