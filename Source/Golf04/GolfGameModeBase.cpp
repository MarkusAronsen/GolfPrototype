// Fill out your copyright notice in the Description page of Project Settings.

#include "GolfGameModeBase.h"

AGolfGameModeBase::AGolfGameModeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AGolfGameModeBase::BeginPlay()
{
	/*UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWallLaser::StaticClass(), laserGrid);
	int counter = 0;
	for (int i = 0; i < laserGrid.Num() / 5; i++)
	{
		for (int j = 0; j < laserGrid.Num() / 3; j++)
		{
			laserGrid[counter]->SetActorLocation(laserGrid[counter]->GetActorLocation() + FVector(0.f, i * 600, j * 600));
			counter++;
		}
	}
	*/

	targetWords.Add(TEXT(""));
	targetWords.Add(TEXT("Rootlads"));
	targetWords.Add(TEXT("Sander Platou Aaseth"));
	targetWords.Add(TEXT("Julie Kristoffersen"));
	targetWords.Add(TEXT("Simon Pettersen"));
	targetWords.Add(TEXT("Markus Strand Aronsen"));
	targetWords.Add(TEXT("Jan Erik Haug"));

	targetRoles.Add(TEXT(""));
	targetRoles.Add(TEXT("A game made by"));
	targetRoles.Add(TEXT("Project manager"));
	targetRoles.Add(TEXT("Graphics and concept artist"));
	targetRoles.Add(TEXT("3D modeler and animator"));
	targetRoles.Add(TEXT("Programmer"));
	targetRoles.Add(TEXT("Team supervisor"));
}

void AGolfGameModeBase::Tick(float DeltaTime)
{
	/*DrawDebugLine(GetWorld(), laserGrid[0]->GetActorLocation(), laserGrid[1]->GetActorLocation(), FColor(255.f, 0.f, 0.f, 1.f));

	gridTimer += DeltaTime;
	if (gridTimer >= 3.f)
	{

		gridTimer = 0.f;
	}
	*/
}
