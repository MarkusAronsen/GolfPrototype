// Fill out your copyright notice in the Description page of Project Settings.

#include "PacmanGhost.h"
#include "GolfBall.h"

// Sets default values
APacmanGhost::APacmanGhost()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacmanGhost::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APacmanGhost::OnBeginOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Pacman ghost no collision box"));

	TArray<AActor*> secretLevelManager;
	UGameplayStatics::GetAllActorsOfClass(this, ASecretLevelManager::StaticClass(), secretLevelManager);
	if (secretLevelManager.Num() > 0)
		secretLevelManagerInstance = Cast<ASecretLevelManager>(secretLevelManager[0]);
	else
		UE_LOG(LogTemp, Warning, TEXT("Secret level manager not found"));


	initialPosition = GetActorLocation();
	initialDirection = direction;
}

// Called every frame
void APacmanGhost::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(pathNode)
		UE_LOG(LogTemp, Warning, TEXT("buffer: %i, distance from path node is %f"), directionBuffer, (GetActorLocation() - pathNode->GetActorLocation()).Size());

	if (directionBuffer != -1 && pathNode && (GetActorLocation() - pathNode->GetActorLocation()).Size() < 20)
	{
		switch (directionBuffer)
		{
		case GBH::directions::UP:
			direction = FVector(1, 0, 0);
			break;
		case GBH::directions::DOWN:
			direction = FVector(-1, 0, 0);
			break;
		case GBH::directions::LEFT:
			direction = FVector(0, -1, 0);
			break;
		case GBH::directions::RIGHT:
			direction = FVector(0, 1, 0);
			break;

		default:
			break;	
		}
		directionBuffer = -1;

	}

	if (!activated)
	{
		//activateTimer += DeltaTime;
		if (secretLevelManagerInstance->activateTimer >= timeToActivate)
		{
			announceGhost(name);
			activated = true;
		}
	}

	if(activated)
	SetActorLocation(GetActorLocation() + direction * DeltaTime * 250);
}

void APacmanGhost::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent,
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()) && !playerIsHit)
	{
		playerIsHit = true;
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->secretLevelManagerInstance->hitGhost();
	}
}

void APacmanGhost::resetGhosts()
{
	SetActorLocation(initialPosition);
	direction = initialDirection;
	activated = false;
	playerIsHit = false;
}