// Fill out your copyright notice in the Description page of Project Settings.

#include "PacmanPellet.h"
#include "GolfBall.h"
#include "SecretLevelManager.h"
#include "GolfGameInstance.h"

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

	UGameplayStatics::GetAllActorsOfClass(this, ASecretLevelManager::StaticClass(), secretLevelManager);
	if (secretLevelManager.Num() > 0)
		secretLevelManagerInstance = Cast<ASecretLevelManager>(secretLevelManager[0]);
	else
		UE_LOG(LogTemp, Warning, TEXT("Could not find secret level manager"));
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
		//UGameplayStatics::PlaySound2D(this, Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->pacmanPickupSound, Cast<UGolfGameInstance>(GetGameInstance())->soundEffectVolume, 1);

		secretLevelManagerInstance->pellets.Remove(this);
		if (secretLevelManagerInstance->pellets.Num() == 0)
		{
			secretLevelManagerInstance->resetPacmanAntiAliasing();
			secretLevelManagerInstance->secretLevelFinished();
		}
		Destroy();
	}
}

