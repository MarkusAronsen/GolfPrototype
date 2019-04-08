	// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "GolfSaveGame.h"

#include "GolfBall.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	checkpointParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Checkpoint"));
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
	TSet<UActorComponent*> components = GetComponents();

	for (auto &element : components)
	{
		if (element->ComponentHasTag("OuterCollision"))
			OuterCollisionBox = Cast<USphereComponent>(element);

		else if (element->ComponentHasTag("InnerCollision"))
			CollisionBox = Cast<USphereComponent>(element);

		else if (element->ComponentHasTag("Mesh"))
			Mesh = Cast<UStaticMeshComponent>(element);
	}

	if (CollisionBox)
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlap);
	else
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint no collision box"));

	if (OuterCollisionBox)
	{
		OuterCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapOuter);
		OuterCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACheckpoint::OnOverlapEndOuter);
	}

	if (Mesh)
		initialZ = Mesh->GetComponentLocation().Z;
	else
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint no mesh"));

	elevateValue = initialZ;

	UParticleSystem* LoadCheckpointParticles = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/GBH/Particles/Particles/Star_Particle.Star_Particle'"));

	if (LoadCheckpointParticles)
		checkpointParticles->SetTemplate(LoadCheckpointParticles);
	else
		UE_LOG(LogTemp, Warning, TEXT("Checkpoint particles not found"));

	checkpointParticles->Deactivate();
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (elevate)
	{
		if (elevateValue < initialZ + 450)
		{
			elevateValue += DeltaTime * 1200;
			Mesh->SetWorldLocation(FVector(Mesh->GetComponentLocation().X, Mesh->GetComponentLocation().Y, elevateValue));
		}
	}

	if (descend)
	{
		if (elevateValue > initialZ)
		{
			elevateValue -= DeltaTime * 1200;
			Mesh->SetWorldLocation(FVector(Mesh->GetComponentLocation().X, Mesh->GetComponentLocation().Y, elevateValue));
		}
	}

}

void ACheckpoint::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		checkpointParticles->Activate();

		UGolfSaveGame* SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));

		if (!UGameplayStatics::DoesSaveGameExist(SaveGameInstance->slotName, SaveGameInstance->userIndex))
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);

		int levelIndex = -1;

		for (int i = 0; i < NUM_LEVELS; i++)
		{
			if (SaveGameInstance->levelData[i].levelName.Compare(UGameplayStatics::GetCurrentLevelName(this), ESearchCase::CaseSensitive) == 0)
			{
				levelIndex = i;
			}
		}

		if (levelIndex != -1)
		{
			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].currentCheckpoint = checkpointIndex;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("Invalid level index (Checkpoint)"));
	}
}

void ACheckpoint::OnOverlapOuter(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	elevate = true;
	descend = false;
}

void ACheckpoint::OnOverlapEndOuter(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	elevate = false;
	descend = true;
}
