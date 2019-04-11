// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbingVolume.h"

// Sets default values
AClimbingVolume::AClimbingVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClimbingVolume::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AClimbingVolume::OnBeginOverlap);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AClimbingVolume::OnEndOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Climbing volume no collision box"));
	}
	
}

// Called every frame
void AClimbingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClimbingVolume::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
		previousState = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state;
}

void AClimbingVolume::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()) && previousState != Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->CLIMBING)
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state = previousState;
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->golfInit(false);
	}
	if (OtherActor->IsA(AGolfBall::StaticClass()) && previousState == Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->CLIMBING)
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->WALKING;
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->golfInit();
	}
}

