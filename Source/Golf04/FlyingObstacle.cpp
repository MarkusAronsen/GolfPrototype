// Fill out your copyright notice in the Description page of Project Settings.

#include "FlyingObstacle.h"
#include "GolfBall.h"

// Sets default values
AFlyingObstacle::AFlyingObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlyingObstacle::BeginPlay()
{
	Super::BeginPlay();

	CollisionBox = FindComponentByClass<UStaticMeshComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFlyingObstacle::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("FlyingObstacle no collision box"));



	/*CollisionBox = this->FindComponentByClass<UBoxComponent>();

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFlyingObstacle::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("FlyingObstacle no collision box"));*/
	
	initialPosition = GetActorLocation();
	initialRotation = GetActorRotation();
}

// Called every frame
void AFlyingObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	translateOffset += DeltaTime + translateSpeed;
	if (translateOffset >= 2 * PI)
		translateOffset = 0.f;

	if (translateX)
		AddActorLocalOffset(FVector(sin(translateOffset) * translateRange, 0, 0));
	if (translateZ)
		AddActorLocalOffset(FVector(0, 0, sin(translateOffset) * translateRange));

	if (rotatePitch)
		AddActorLocalRotation(FRotator(rotateSpeed * DeltaTime, 0, 0));

	if (startResetTimer)
		resetTimer += DeltaTime;
		if (resetTimer >= 1.f)
		{
			startResetTimer = false;
			resetTimer = 0.f;
			resetFlyingObstacle();
		}
}

void AFlyingObstacle::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		if (!Cast<AGolfBall>(OtherActor)->bRespawning)
		{
			static_cast<AGolfBall*>(OtherActor)->state = static_cast<AGolfBall*>(OtherActor)->states::WALKING;
			static_cast<AGolfBall*>(OtherActor)->golfInit();
			static_cast<AGolfBall*>(OtherActor)->respawnAtCheckpoint();
			Cast<AGolfBall>(OtherActor)->FlyingScoreWidget->SetVisibility(ESlateVisibility::Hidden);
			Cast<AGolfBall>(OtherActor)->flyingRestarts++;
		}
	}
}

void AFlyingObstacle::resetFlyingObstacle()
{
	SetActorLocation(initialPosition);
	SetActorRotation(initialRotation);
	translateOffset = 0.f;
}

