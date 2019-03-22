// Fill out your copyright notice in the Description page of Project Settings.

#include "PacmanPellet.h"
#include "GolfBall.h"

// Sets default values
APacmanPellet::APacmanPellet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacmanPellet::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APacmanPellet::OnBeginOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Pacman pellet no collision box"));
}

// Called every frame
void APacmanPellet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APacmanPellet::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->secretLevelManagerInstance->pacmanScore += 10;
		Destroy();
	}
}

