// Fill out your copyright notice in the Description page of Project Settings.

#include "PacmanTeleporter.h"
#include "GolfBall.h"
#include "PacmanGhost.h"

// Sets default values
APacmanTeleporter::APacmanTeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APacmanTeleporter::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APacmanTeleporter::OnBeginOverlap);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pacman teleporter no collision box"));
	}
}

// Called every frame
void APacmanTeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APacmanTeleporter::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		//Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorRotation(Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->GetActorForwardVector().Rotation() * -1);
		
		if (leftTeleporter)
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(FVector(-7, 1350, 113));
		else if(rightTeleporter)
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation(FVector(-7, -1350, 113));
	}

	if (OtherActor->IsA(APacmanGhost::StaticClass()))
	{
		//Cast<APacmanGhost>(OtherActor)->direction = Cast<APacmanGhost>(OtherActor)->direction * -1;

		if (leftTeleporter)
			Cast<APacmanGhost>(OtherActor)->SetActorLocation(FVector(-7, 1350, 113));
		else if (rightTeleporter)
			Cast<APacmanGhost>(OtherActor)->SetActorLocation(FVector(-7, -1350, 113));
	}
	
		//Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->SetActorLocation()
}

