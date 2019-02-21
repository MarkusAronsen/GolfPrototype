// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingObstacle.h"


// Sets default values
AFlyingObstacle::AFlyingObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlyingObstacle::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UBoxComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFlyingObstacle::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FlyingObstacle no collision box"));
	}
	
}

// Called every frame
void AFlyingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlyingObstacle::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		static_cast<AGolfBall*>(OtherActor)->state = static_cast<AGolfBall*>(OtherActor)->states::WALKING;
		static_cast<AGolfBall*>(OtherActor)->golfInit();
		static_cast<AGolfBall*>(OtherActor)->respawnAtCheckpoint();
	}
}

