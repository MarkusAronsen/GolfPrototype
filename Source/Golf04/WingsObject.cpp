// Fill out your copyright notice in the Description page of Project Settings.

#include "WingsObject.h"

// Sets default values
AWingsObject::AWingsObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"), true);
	CollisionBox->SetBoxExtent(FVector(20, 10, 20));
}

// Called when the game starts or when spawned
void AWingsObject::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AWingsObject::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Golf Object no collision box"));

}

// Called every frame
void AWingsObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWingsObject::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()) && static_cast<AGolfBall*>(OtherActor)->state != static_cast<AGolfBall*>(OtherActor)->states::FLYING)
	{
		static_cast<AGolfBall*>(OtherActor)->state = static_cast<AGolfBall*>(OtherActor)->states::FLYING;
		static_cast<AGolfBall*>(OtherActor)->flyingInit(this);
		static_cast<AGolfBall*>(OtherActor)->lerpTimer = 0.f;
	}
}