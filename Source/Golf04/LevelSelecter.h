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
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapBeginOuter(UPrimitiveComponent* OverlappedComponent, AActor *OtherActor,
			UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult &SweepResult);


	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* mCollisionBox = nullptr;

	UPROPERTY(Category = "Component", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* mOuterCollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Level Name")
		FString levelName;

	UPROPERTY(Category = "Widget", EditAnywhere, BlueprintReadWrite)
		UUserWidget* LevelSelectWidget = nullptr;

	UPROPERTY(Category = "Widget", EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UUserWidget> LevelSelectWidget_BP;

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom event")
		void overlapCustomEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "Custom event")
		void outerOverlapCustomEvent();
	//Move to level data read/write class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level performance")
		int levelPerformance = 0;
};