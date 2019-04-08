// Fill out your copyright notice in the Description page of Project Settings.

#include "Goal.h"
#include "GolfBall.h"


// Sets default values
AGoal::AGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	goalParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Goal"));
	goalParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGoal::BeginPlay()
{
	Super::BeginPlay();

	//CollisionBox = this->FindComponentByClass<USphereComponent>();

	TSet<UActorComponent*> components = GetComponents();

	for (auto& element : components)
	{
		if (element->ComponentHasTag("InnerCollision"))
			CollisionBox = Cast<USphereComponent>(element);

		else if (element->ComponentHasTag("OuterCollision"))
			OuterCollisionBox = Cast<USphereComponent>(element);

		else if (element->ComponentHasTag("Mesh"))
			Mesh = Cast<UStaticMeshComponent>(element);
	}

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlap);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AGoal::OnEndOverlap);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Goal no collision box"));
	
	if (OuterCollisionBox)
	{
		OuterCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGoal::OnOverlapOuter);
		OuterCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AGoal::OnEndOverlapOuter);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Goal no outer collision box"));

	if (Mesh)
		initialZ = Mesh->GetComponentLocation().Z;
	else
		UE_LOG(LogTemp, Warning, TEXT("Goal no mesh"));

	levelName = UGameplayStatics::GetCurrentLevelName(this);

	elevateValue = initialZ;

	UParticleSystem* LoadGoalParticles = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/GBH/Particles/Particles/Star_Particle.Star_Particle'"));

	if (LoadGoalParticles)
		goalParticles->SetTemplate(LoadGoalParticles);
	else
		UE_LOG(LogTemp, Warning, TEXT("Goal particles not found"));

	goalParticles->SetWorldLocation(goalParticles->GetComponentLocation() + FVector(0, 0, 350));
}

// Called every frame
void AGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (startSettleTimer)
	{
		settleTimer += DeltaTime;
		if (settleTimer >= 2.5f)
		{
			settleTimer = 0.f;
			startSettleTimer = false;
			levelTimeElapsed = UGameplayStatics::GetUnpausedTimeSeconds(this);
			saveLevelData();
			UGameplayStatics::OpenLevel(this, "LevelSelect");
		}
	}

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

	//UE_LOG(LogTemp, Warning, TEXT("ElevateValue: %f, initialZ: %f"), elevateValue, initialZ);
}

void AGoal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
	UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		settleTimer = 0.f;
		startSettleTimer = true;
		goalParticles->Activate();
	}
}

void AGoal::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	startSettleTimer = false;
	settleTimer = 0.f;
}

void AGoal::OnOverlapOuter(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult & SweepResult)
{
	elevate = true;
	descend = false;
}

void AGoal::OnEndOverlapOuter(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	elevate = false;
	descend = true;
}

void AGoal::saveLevelData()
{
	UGolfSaveGame* SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));
	UGolfSaveGame* LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::CreateSaveGameObject(UGolfSaveGame::StaticClass()));

	if (!UGameplayStatics::DoesSaveGameExist(SaveGameInstance->slotName, SaveGameInstance->userIndex))
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);

	LoadGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->slotName, LoadGameInstance->userIndex));

	int levelIndex = -1;

	for (int i = 0; i < NUM_LEVELS; i++)
	{
		if (SaveGameInstance->levelData[i].levelName.Compare(levelName, ESearchCase::CaseSensitive) == 0)
		{
			levelIndex = i;
		}
	}

	if (levelIndex != -1)
	{
		if (LoadGameInstance->levelData[levelIndex].timeElapsed > levelTimeElapsed || LoadGameInstance->levelData[levelIndex].timeElapsed < 0)
		{
			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].timeElapsed = levelTimeElapsed;
			SaveGameInstance->levelData[levelIndex].currentCheckpoint = -1;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
		}

			SaveGameInstance = Cast<UGolfSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->slotName, SaveGameInstance->userIndex));
			SaveGameInstance->levelData[levelIndex].currentCheckpoint = -1;
			SaveGameInstance->levelData[levelIndex].bLevelCompleted = true;
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->slotName, SaveGameInstance->userIndex);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Invalid level index"));
}
