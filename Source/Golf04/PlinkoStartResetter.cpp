// Fill out your copyright notice in the Description page of Project Settings.

#include "PlinkoStartResetter.h"
#include "GolfBall.h"

// Sets default values
APlinkoStartResetter::APlinkoStartResetter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlinkoStartResetter::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = FindComponentByClass<UShapeComponent>();
	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlinkoStartResetter::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("PlinkoStartResetter no collision box"));
}

// Called every frame
void APlinkoStartResetter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlinkoStartResetter::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		if (Cast<AGolfBall>(OtherActor)->secretLevelManagerInstance)
			Cast<AGolfBall>(OtherActor)->secretLevelManagerInstance->plinkoLaunchReady = true;
	}
}

