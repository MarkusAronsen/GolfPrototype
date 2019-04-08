// Fill out your copyright notice in the Description page of Project Settings.

#include "DestructableBlock.h"

// Sets default values
ADestructableBlock::ADestructableBlock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructableBlock::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADestructableBlock::OnBeginOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DestructableBlock no collision box"));
	}

	maxHitPoints = hitPoints;

	startPos = GetActorLocation();
}

// Called every frame
void ADestructableBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hitPoints <= 0.f)
	{
		//SetActorLocation(FVector(0.f, 0.f, -1000.f));
		bDead = true;
	}

	if (bDead)
	{
		if (deadTimer >= 0.05f)
		{
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
		}
		deadTimer += DeltaTime;
	}
}

void ADestructableBlock::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		hitPoints -= Cast<AGolfBall>(OtherActor)->mMesh->GetPhysicsLinearVelocity().Size();
	}
}

void ADestructableBlock::resetFunction()
{
	hitPoints = maxHitPoints;
	bDead = false;
	deadTimer = 0.f;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

