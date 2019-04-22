// Fill out your copyright notice in the Description page of Project Settings.

#include "BilliardsHole.h"
#include "GolfBall.h"
#include "BilliardBall.h"

// Sets default values
ABilliardsHole::ABilliardsHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABilliardsHole::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<USphereComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ABilliardsHole::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Billiards hole no collision box"));
}

// Called every frame
void ABilliardsHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
/*
	if (startSettleTimer)
	{
		settleTimer += DeltaTime;
		if (settleTimer >= 2.f)
		{
			if (Cast<ABilliardBall>(OtherBillardBall)->is8Ball)
				Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->secretLevelManagerInstance->secretLevelFinished(true);

			OtherBillardBall->Destroy();
			settleTimer = 0.f;
			startSettleTimer = false;
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->secretLevelManagerInstance->registerBilliards();

			UGameplayStatics::GetAllActorsOfClass(this, ABilliardBall::StaticClass(), BilliardBalls);
			if (BilliardBalls.Num() == 1)
				Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->secretLevelManagerInstance->secretLevelFinished();
		}

	}
	*/
}

void ABilliardsHole::OnOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(ABilliardBall::StaticClass()))
	{
		startSettleTimer = true;
		settleTimer = 0.f;
		OtherBillardBall = OtherActor;
	}
}

