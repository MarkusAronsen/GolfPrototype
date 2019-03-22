// Fill out your copyright notice in the Description page of Project Settings.

#include "PacmanPathNode.h"
#include "GolfBall.h"
#include "PacmanGhost.h"


// Sets default values
APacmanPathNode::APacmanPathNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacmanPathNode::BeginPlay()
{
	Super::BeginPlay();

	srand(time(nullptr));

	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APacmanPathNode::OnBeginOverlap);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APacmanPathNode::OnEndOverlap);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pacman path node no collision box"));
	}
	
}

// Called every frame
void APacmanPathNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//drawDebugObjects();

}

void APacmanPathNode::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		UGameplayStatics::GetAllActorsOfClass(this, ASecretLevelManager::StaticClass(), secretLevelManager);
		Cast<ASecretLevelManager>(secretLevelManager[0])->overlappingNode = true;
		Cast<ASecretLevelManager>(secretLevelManager[0])->pacmanNode = this;
	}

	if (OtherActor->IsA(APacmanGhost::StaticClass()))
	{
		Cast<APacmanGhost>(OtherActor)->pathNode = this;

		if (up)
			Cast<APacmanGhost>(OtherActor)->legalDirections.Add(Cast<APacmanGhost>(OtherActor)->GBH::directions::UP);
		if (down)
			Cast<APacmanGhost>(OtherActor)->legalDirections.Add(Cast<APacmanGhost>(OtherActor)->GBH::directions::DOWN);
		if (left)
			Cast<APacmanGhost>(OtherActor)->legalDirections.Add(Cast<APacmanGhost>(OtherActor)->GBH::directions::LEFT);
		if (right)
			Cast<APacmanGhost>(OtherActor)->legalDirections.Add(Cast<APacmanGhost>(OtherActor)->GBH::directions::RIGHT);

		Cast<APacmanGhost>(OtherActor)->directionBuffer = Cast<APacmanGhost>(OtherActor)->legalDirections[rand() % Cast<APacmanGhost>(OtherActor)->legalDirections.Num()];

		Cast<APacmanGhost>(OtherActor)->legalDirections.Empty();
	}
}

void APacmanPathNode::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		UGameplayStatics::GetAllActorsOfClass(this, ASecretLevelManager::StaticClass(), secretLevelManager);
		Cast<ASecretLevelManager>(secretLevelManager[0])->overlappingNode = false;
	}
}

void APacmanPathNode::drawDebugObjects()
{
	if(up)
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(150, 0, 0), FColor::Red, false, 0, (uint8)'\000', 10.f);
	if(down)
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(-150, 0, 0), FColor::Blue, false, 0, (uint8)'\000', 10.f);
	if(left)
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, -150, 0), FColor::Yellow, false, 0, (uint8)'\000', 10.f);
	if(right)
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector(0, 150, 0), FColor::Green, false, 0, (uint8)'\000', 10.f);
}