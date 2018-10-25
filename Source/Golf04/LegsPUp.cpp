// Fill out your copyright notice in the Description page of Project Settings.

#include "LegsPUp.h"


// Sets default values
ALegsPUp::ALegsPUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALegsPUp::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = this->FindComponentByClass<USphereComponent>();

	/*if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALegsPUp::OnOverlap);
	}*/
	
}

// Called every frame
void ALegsPUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*void ALegsPUp::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	//empty
}*/

