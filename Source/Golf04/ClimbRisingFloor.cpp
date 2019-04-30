// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbRisingFloor.h"

#include "GolfBall.h"

// Sets default values
AClimbRisingFloor::AClimbRisingFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClimbRisingFloor::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = this->FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AClimbRisingFloor::OnBeginOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("ClimbRisingFloor no collision box"));

	startPos = GetActorLocation();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	TArray<AActor*> golfBalls;
	UGameplayStatics::GetAllActorsOfClass(this, AGolfBall::StaticClass(), golfBalls);
	if(golfBalls.Num() > 0)
		GolfBallPtr = Cast<AGolfBall>(golfBalls[0]);

}

// Called every frame
void AClimbRisingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(Cast<AGolfBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->state == Cast<AGolfBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->CLIMBING)
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), DeltaTime, 0.035f));

	if (reseting)
	{
		timer += DeltaTime;
		if (timer > 1.f)
		{
			SetActorLocation(startPos);
			timer = 0.f;
			reseting = false;
		}
	}

	if (receding && oceanActivated)
	{
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), startPos, DeltaTime, 0.5f));
		if (GetActorLocation().Z < startPos.Z + 10.f)
		{
			receding = false;
		}
	}

	if (GolfBallPtr)
	{
		if (GolfBallPtr->state == GolfBallPtr->CLIMBING && !oceanActivated)
			oceanActivated = true;

		//44000 er Z verdien til en av de høyeste climbing nodene i siste level. Hvis man er under denne og ikke i climbing skal haved trekke seg tilbake til start.
		if (!reseting && oceanActivated && GolfBallPtr->state != GolfBallPtr->CLIMBING && GolfBallPtr->GetActorLocation().Z < 44000.f)
		{
			SetActorLocation(FMath::VInterpTo(GetActorLocation(), startPos, DeltaTime, 0.5f));
		}
	}


}

void AClimbRisingFloor::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()) && !reseting)
	{
		GolfBallPtr->respawnAtCheckpoint();
		GolfBallPtr->mMesh->SetSimulatePhysics(true);
		GolfBallPtr->mMesh->SetLinearDamping(9.f);
		reseting = true;
	}
}

