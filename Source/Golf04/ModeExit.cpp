// Fill out your copyright notice in the Description page of Project Settings.

#include "ModeExit.h"

// Sets default values
AModeExit::AModeExit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"), true);
	CollisionBox->SetBoxExtent(FVector(100, 100, 100));
}

// Called when the game starts or when spawned
void AModeExit::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AModeExit::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level selecter no collision box"));
	}
}

// Called every frame
void AModeExit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AModeExit::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		static_cast<AGolfBall*>(OtherActor)->state = static_cast<AGolfBall*>(OtherActor)->states::GOLF;
		static_cast<AGolfBall*>(OtherActor)->golfInit();

	}
}

