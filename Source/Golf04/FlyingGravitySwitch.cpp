// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingGravitySwitch.h"
#include "GolfBall.h"

// Sets default values
AFlyingGravitySwitch::AFlyingGravitySwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlyingGravitySwitch::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFlyingGravitySwitch::OnBeginOverlap);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Gravity switch no collision box"));

	mesh = FindComponentByClass<UStaticMeshComponent>();
}

// Called every frame
void AFlyingGravitySwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ignore)
	{
		ignoreTimer += DeltaTime;
		if (ignoreTimer > 2.f)
		{
			ignoreTimer = 0.f;
			ignore = false;
		}
	}
}

void AFlyingGravitySwitch::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent,
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()) && !ignore)
	{
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->flyingGravityFlipped = !Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->flyingGravityFlipped;
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->easeGravityShift = true;
		ignore = true;
		ignoreTimer = 0.f;
		mesh->SetVisibility(false);
	}
}

