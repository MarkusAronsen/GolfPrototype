// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpandingFloor.h"

// Sets default values
AExpandingFloor::AExpandingFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExpandingFloor::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox = this->FindComponentByClass<UBoxComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AExpandingFloor::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Expanding floor no collision box"));
	}
}

// Called every frame
void AExpandingFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (isExpanding)
	{
		if (GetActorScale3D().X > 20.f)
			isExpanding = false;

		SetActorRelativeScale3D(FVector(scaleFactor, scaleFactor, 1.f));
		scaleFactor += DeltaTime * 100.f;
	}

}

void AExpandingFloor::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		if(GetActorScale3D().X < 20.f)
		{ 
			isExpanding = true;
			static_cast<AGolfBall*>(OtherActor)->state = static_cast<AGolfBall*>(OtherActor)->states::WALKING;
			static_cast<AGolfBall*>(OtherActor)->golfInit();
			CollisionBox->SetRelativeScale3D(FVector(1.f, 1.f, 1.f));
		}

		if (bPush)
			static_cast<AGolfBall*>(OtherActor)->SetActorLocation(static_cast<AGolfBall*>(OtherActor)->GetActorLocation() + FVector(0.f, 0.f, 300.f));
		
		bPush = false;
	}
}

