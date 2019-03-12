// Fill out your copyright notice in the Description page of Project Settings.

#include "SecretLevelEntrance.h"
#include "GolfBall.h"

// Sets default values
ASecretLevelEntrance::ASecretLevelEntrance()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASecretLevelEntrance::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<UCapsuleComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASecretLevelEntrance::OnOverlap);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Secret level entrance no collision box"));

	}

	
}

// Called every frame
void ASecretLevelEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASecretLevelEntrance::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		UGameplayStatics::OpenLevel(this, FName(*SecretLevelName));
	}
}