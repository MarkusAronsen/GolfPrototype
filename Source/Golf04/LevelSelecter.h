// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "GolfBall.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSelecter.generated.h"

UCLASS()
class GOLF04_API ALevelSelecter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSelecter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* mCollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Level Name")
		FName levelName;

	UPROPERTY(Category = "Widget", EditAnywhere, BlueprintReadWrite)
		UUserWidget* LevelSelectWidget = nullptr;

	UPROPERTY(Category = "Widget", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> LevelSelectWidget_BP;


	//Move to file read/write system
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int levelPerformance = 0;
};
