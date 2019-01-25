// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelecter.h"

// Sets default values
ALevelSelecter::ALevelSelecter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"), true);
	mCollisionBox->SetSphereRadius(100, true);
}

// Called when the game starts or when spawned
void ALevelSelecter::BeginPlay()
{
	Super::BeginPlay();

	if (mCollisionBox)
	{
		mCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelSelecter::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level selecter no collision box"));
	}


}

// Called every frame
void ALevelSelecter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSelecter::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		UGameplayStatics::OpenLevel(GetWorld(), levelName);
	}
}