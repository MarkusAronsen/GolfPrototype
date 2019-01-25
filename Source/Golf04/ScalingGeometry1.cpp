// Fill out your copyright notice in the Description page of Project Settings.

#include "ScalingGeometry1.h"

// Sets default values
AScalingGeometry1::AScalingGeometry1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AScalingGeometry1::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScalingGeometry1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	timeToScale += DeltaTime;

		if (timeToScale > 3.f && scaleUp)
		{
			SetActorScale3D(FVector(scaleXY, scaleXY, scaleZ));
			scaleZ += 0.008f;
			scaleXY += 0.016f;
			if (scaleZ > 2.f)
			{
				scaleUp = false;
				timeToScale = 0.f;
			}
		}

		else if (timeToScale > 3.f && !scaleUp)
		{
			SetActorScale3D(FVector(scaleXY, scaleXY, scaleZ));
			scaleZ -= 0.008f;
			scaleXY -= 0.016f;
			if (scaleZ < 1.f)
			{
				scaleUp = true;
				timeToScale = 0.f;
			}
		}
}

