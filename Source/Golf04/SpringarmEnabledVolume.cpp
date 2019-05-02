// Fill out your copyright notice in the Description page of Project Settings.

#include "SpringarmEnabledVolume.h"
#include "GolfBall.h"

// Sets default values
ASpringarmEnabledVolume::ASpringarmEnabledVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpringarmEnabledVolume::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpringarmEnabledVolume::OnBeginOverlap);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ASpringarmEnabledVolume::OnEndOverlap);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Spring Arm Volume no collison box!"));

}

// Called every frame
void ASpringarmEnabledVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpringarmEnabledVolume::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		Cast<AGolfBall>(OtherActor)->mSpringArm->bDoCollisionTest = true;
	}
}

void ASpringarmEnabledVolume::OnEndOverlap(UPrimitiveComponent * OverlappedComponent,
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		Cast<AGolfBall>(OtherActor)->mSpringArm->bDoCollisionTest = false;
	}
}

