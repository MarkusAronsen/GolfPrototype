// Fill out your copyright notice in the Description page of Project Settings.

#include "XZGravityWall.h"

// Sets default values
AXZGravityWall::AXZGravityWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"), true);

}

// Called when the game starts or when spawned
void AXZGravityWall::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AXZGravityWall::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Gravity wall no collision box"));
	}

	CollisionBox->SetBoxExtent(FVector(675, 100, 657), true);
	
}

// Called every frame
void AXZGravityWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXZGravityWall::OnOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		//mMesh->BodyInstance.CustomDOFPlaneNormal = GetActorForwardVector().RotateAngleAxis(90.f, FVector(0.f, 1.f, 0.f));

		OtherActor->FindComponentByClass<UStaticMeshComponent>()->BodyInstance.CustomDOFPlaneNormal = GetActorRightVector();
		//OtherActor->FindComponentByClass<UStaticMeshComponent>()->BodyInstance.SetDOFLock(EDOFMode::CustomPlane);
		OtherActor->FindComponentByClass<UStaticMeshComponent>()->SetConstraintMode(EDOFMode::CustomPlane);

		UE_LOG(LogTemp, Warning, TEXT("Hit player"));
	}
}

