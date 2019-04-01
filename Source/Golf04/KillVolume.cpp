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
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->respawnAtCheckpoint();
	}
}

