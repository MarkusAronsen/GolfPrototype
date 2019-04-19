// Fill out your copyright notice in the Description page of Project Settings.

#include "ClimbObject.h"


// Sets default values
AClimbObject::AClimbObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AClimbObject::BeginPlay()
{
	Super::BeginPlay();
	
	/*TArray<FHitResult> hitResults;

	FCollisionShape sphereTrace = FCollisionShape::MakeSphere(32.f);
	FCollisionQueryParams params = FCollisionQueryParams::DefaultQueryParam;
	params.AddIgnoredActor(this);
	//params.ClearIgnoredComponents();

	GetWorld()->SweepMultiByChannel(
		hitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,
		sphereTrace,
		params);

	for (int i = 0; i < hitResults.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *hitResults[i].GetActor()->GetHumanReadableName());
			
		if(hitResults[i].GetActor()->GetName().Contains("Cube") && !bIsEdgeNode)
			SetActorRotation(hitResults[i].ImpactNormal.Rotation());

		UE_LOG(LogTemp, Warning, TEXT("%s"), *GetActorLocation().ToString());
	}

        if(!bIsEdgeNode)
			SetActorLocation(GetActorLocation() + GetActorForwardVector() * 50);
			*/

	CollisionBox = FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		TSet<AActor*> overlappingActors;

		//Actors should generate overlap events
		CollisionBox->GetOverlappingActors(overlappingActors);
		//UE_LOG(LogTemp, Warning, TEXT("%i"), overlappingActors.Num());


		if (overlappingActors.Num() > 0)
		{
			for (auto &elem : overlappingActors)
			{
				if (elem->GetName().Contains("Cube"))
				{
					SetActorRotation((elem->GetActorForwardVector() * -1).Rotation());
					if (!bIsEdgeNode)
						SetActorLocation(GetActorLocation() + GetActorForwardVector() * 50);
				}
			}
		}
	}

}

// Called every frame
void AClimbObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ignored)
	{
		ignoreTimer += DeltaTime;
		if (ignoreTimer >= 0.2f)
		{
			ignoreTimer = 0.f;
			ignored = false;
		}
	}
}

