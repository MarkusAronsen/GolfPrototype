// Fill out your copyright notice in the Description page of Project Settings.

#include "PlinkoBlocker.h"
#include "GolfBall.h"

// Sets default values
APlinkoBlocker::APlinkoBlocker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlinkoBlocker::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlinkoBlocker::OnBeginOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Plinko blocker no collision box"));

	Mesh = FindComponentByClass<UStaticMeshComponent>();
	if(!Mesh)
		UE_LOG(LogTemp, Warning, TEXT("Plinko blocker no mesh"));

	initialLocationZ = Mesh->GetComponentLocation().Z;
	ascendValue = initialLocationZ;
}

// Called every frame
void APlinkoBlocker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ascend)
	{
		if (ascendValue < initialLocationZ + 1000)
		{
			ascendValue += DeltaTime * 2000;
			Mesh->SetWorldLocation(FVector(Mesh->GetComponentLocation().X, Mesh->GetComponentLocation().Y, ascendValue));
		}
	}
}

void APlinkoBlocker::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult)
{
	ascend = true;
}

void APlinkoBlocker::reset()
{
	ascend = false;
	ascendValue = initialLocationZ;
	Mesh->SetWorldLocation(FVector(Mesh->GetComponentLocation().X, Mesh->GetComponentLocation().Y, initialLocationZ));
}