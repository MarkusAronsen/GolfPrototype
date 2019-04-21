// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingGolfBall.h"

// Sets default values
AMovingGolfBall::AMovingGolfBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AMovingGolfBall::BeginPlay()
{
	Super::BeginPlay();

	srand(time(nullptr));

	mesh = FindComponentByClass<UStaticMeshComponent>();
	
	if (!mesh)
		UE_LOG(LogTemp, Warning, TEXT("Moving golfball no mesh"));
	
	mesh->SetSimulatePhysics(true);

}

// Called every frame
void AMovingGolfBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	launchTimer += DeltaTime;

	if (launchTimer >= launchInterval)
	{
		mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);

		launchTimer = 0.f;

		FVector launchVector = FVector(FMath::RandRange(-1, 1), FMath::RandRange(-1, 1), 0);
		launchVector.Normalize();
		launchVector *= launchForce;

		mesh->AddImpulse(launchVector);
		UE_LOG(LogTemp, Warning, TEXT("Linear velocity: %s, launchVector: %s"), *mesh->GetPhysicsLinearVelocity().ToString(), *launchVector.ToString());

	}
}

