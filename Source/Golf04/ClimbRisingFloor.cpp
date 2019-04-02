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
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AClimbRisingFloor::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("ClimbRisingFloor no collision box"));
}

// Called every frame
void AClimbRisingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Cast<AGolfBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->state == Cast<AGolfBall>(GetWorld()->GetFirstPlayerController()->GetPawn())->CLIMBING)
		SetActorLocation(FMath::VInterpTo(GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), DeltaTime, 0.03f));

}

void AClimbRisingFloor::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		AGolfBall *GolfBallPtr = Cast<AGolfBall>(OtherActor);

		GolfBallPtr->state = GolfBallPtr->WALKING;
		GolfBallPtr->golfInit();

	}
}

