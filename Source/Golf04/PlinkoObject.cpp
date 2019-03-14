// Fill out your copyright notice in the Description page of Project Settings.

#include "PlinkoObject.h"
#include "GolfBall.h"

// Sets default values
APlinkoObject::APlinkoObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlinkoObject::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UBoxComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APlinkoObject::OnOverlapBegin);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &APlinkoObject::OnOverlapEnd);
	}	
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Plinko object no collision box"));

	}
}

// Called every frame
void APlinkoObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (overlapped)
	{
		settleTimer += DeltaTime;
		if (settleTimer >= 3.f)
		{
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->secretLevelManagerInstance->registerPlinkoScore(scoreValue);
			overlapped = false;
			settleTimer = 0.f;
		}
	}

}

void APlinkoObject::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		settleTimer = 0.f;
		overlapped = true;
	}
}

void APlinkoObject::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, 
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		settleTimer = 0.f;
		overlapped = false;
	}

}

