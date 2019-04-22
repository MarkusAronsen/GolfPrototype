/*// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsMazeHole.h"
#include "PhysicsMaze.h"

// Sets default values
APhysicsMazeHole::APhysicsMazeHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APhysicsMazeHole::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> mazeArr;

	UGameplayStatics::GetAllActorsOfClass(this, APhysicsMaze::StaticClass(), mazeArr);

	if (mazeArr.Num() == 1)
		mazePtr = Cast<APhysicsMaze>(mazeArr[0]);
	else
		UE_LOG(LogTemp, Warning, TEXT("Maze not initialized"));

	AttachToActor(mazePtr, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
}

// Called every frame
void APhysicsMazeHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
*/