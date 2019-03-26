// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelecter.h"

// Sets default values
ALevelSelecter::ALevelSelecter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"), true);
	mCollisionBox->SetSphereRadius(100, true);

	mOuterCollisionBox = CreateDefaultSubobject<USphereComponent>(TEXT("OuterCollision"), true);
	mOuterCollisionBox->SetSphereRadius(1000, true);
}

// Called when the game starts or when spawned
void ALevelSelecter::BeginPlay()
{
	Super::BeginPlay();

	if (mCollisionBox)
	{
		mCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelSelecter::OnOverlapBegin);
		mCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ALevelSelecter::OnOverlapEnd);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level selecter no collision box"));
	}

	if (mOuterCollisionBox)
	{
		mOuterCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ALevelSelecter::OnOverlapBeginOuter);
		mOuterCollisionBox->SetWorldLocation(GetActorLocation());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Level selecter outer no collision box"));
	}

	if (LevelSelectWidget_BP)
	{
		LevelSelectWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), LevelSelectWidget_BP, FName(*levelName));
		if (LevelSelectWidget)
		{
			LevelSelectWidget->AddToViewport();
			LevelSelectWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("LevelSelectWidget not initialized"));
}

// Called every frame
void ALevelSelecter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALevelSelecter::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		overlapCustomEvent();
		LevelSelectWidget->SetVisibility(ESlateVisibility::Visible);

		if (!Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0)))
			UE_LOG(LogTemp, Warning, TEXT("Cast to player pawn failed"));

		//Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->mMesh->SetSimulatePhysics(false);
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::AWAITING_LEVELSELECT_INPUT;
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->setLevelToOpen(levelName);
		Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->currentLevelSelecter = this;
		//UGameplayStatics::OpenLevel(this, levelName);
	}
}

void ALevelSelecter::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		LevelSelectWidget->SetVisibility(ESlateVisibility::Hidden);

		if (Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0)))
		{
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->state = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->states::WALKING;
			Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0))->currentLevelSelecter = nullptr;
		}
	}
}

void ALevelSelecter::OnOverlapBeginOuter(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		outerOverlapCustomEvent();
	}
}
