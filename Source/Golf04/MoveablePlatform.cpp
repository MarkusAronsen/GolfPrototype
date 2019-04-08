// Fill out your copyright notice in the Description page of Project Settings.

#include "MoveablePlatform.h"
#include "GolfBall.h"

// Sets default values
AMoveablePlatform::AMoveablePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMoveablePlatform::BeginPlay()
{
	Super::BeginPlay();

	TSet<UActorComponent*> components = GetComponents();

	for (auto& element : components)
	{
		if (element->ComponentHasTag("Up"))
		{
			Up = Cast<UStaticMeshComponent>(element);
			Up->OnComponentBeginOverlap.AddDynamic(this, &AMoveablePlatform::OnOverlap);
			Up->OnComponentEndOverlap.AddDynamic(this, &AMoveablePlatform::OnEndOverlap);
		}

		else if (element->ComponentHasTag("Down"))
		{
			Down = Cast<UStaticMeshComponent>(element);
			Down->OnComponentBeginOverlap.AddDynamic(this, &AMoveablePlatform::OnOverlap);
			Down->OnComponentEndOverlap.AddDynamic(this, &AMoveablePlatform::OnEndOverlap);
		}

		else if (element->ComponentHasTag("Left"))
		{
			Left = Cast<UStaticMeshComponent>(element);
			Left->OnComponentBeginOverlap.AddDynamic(this, &AMoveablePlatform::OnOverlap);
			Left->OnComponentEndOverlap.AddDynamic(this, &AMoveablePlatform::OnEndOverlap);
		}

		else if (element->ComponentHasTag("Right"))
		{
			Right = Cast<UStaticMeshComponent>(element);
			Right->OnComponentBeginOverlap.AddDynamic(this, &AMoveablePlatform::OnOverlap);
			Right->OnComponentEndOverlap.AddDynamic(this, &AMoveablePlatform::OnEndOverlap);
		}

		else if (element->ComponentHasTag("Pad"))
		{
			Pad = Cast<UStaticMeshComponent>(element);
			RootComponent = Pad;
		}
	}
	
	CollisionBox = FindComponentByClass<UShapeComponent>();

	if (CollisionBox)
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMoveablePlatform::OnOverlap);
		CollisionBox->OnComponentEndOverlap.AddDynamic(this, &AMoveablePlatform::OnEndOverlap);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("MoveablePlatform no collision box"));

}

// Called every frame
void AMoveablePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (upPushed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Adding offset up"));

		//Pad->SetPhysicsLinearVelocity(GetActorForwardVector() * DeltaTime * moveSpeed);
		//if (playerPtr)
			//playerPtr->mMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * DeltaTime * moveSpeed);

		AddActorLocalOffset(GetActorForwardVector() * DeltaTime * moveSpeed);
		//Pad->AddForce(GetActorForwardVector() * DeltaTime * moveSpeed, NAME_None, true);
		//SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * moveSpeed, true);
		//if (playerPtr)
			//playerPtr->AddActorLocalOffset(GetActorForwardVector() * DeltaTime * moveSpeed);

			//playerPtr->SetActorLocation(playerPtr->GetActorLocation() + GetActorForwardVector() * DeltaTime * moveSpeed, true);
			//playerPtr->mMesh->AddForce(GetActorForwardVector() * DeltaTime * moveSpeed, NAME_None, true);
	}

	if (downPushed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Adding offset down"));
		//Pad->SetPhysicsLinearVelocity(GetActorForwardVector() * -1 * DeltaTime * moveSpeed);
		//if (playerPtr)
			//playerPtr->mMesh->SetPhysicsLinearVelocity(GetActorForwardVector() * -1 * DeltaTime * moveSpeed);

		AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * moveSpeed);
		//Pad->AddForce(GetActorForwardVector() * -1 * DeltaTime * moveSpeed, NAME_None, true);
		//SetActorLocation(GetActorLocation() + GetActorForwardVector() * -1 * DeltaTime * moveSpeed, true);
		//if (playerPtr)
			//playerPtr->AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * moveSpeed);

			//playerPtr->SetActorLocation(playerPtr->GetActorLocation() + GetActorForwardVector() * -1 * DeltaTime * moveSpeed, true);
			//playerPtr->mMesh->AddForce(GetActorForwardVector() * -1 * DeltaTime * moveSpeed, NAME_None, true);
	}

	if (leftPushed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Adding offset left"));
		//Pad->SetPhysicsLinearVelocity(GetActorRightVector() * -1 * DeltaTime * moveSpeed);
		//if (playerPtr)
			//playerPtr->mMesh->SetPhysicsLinearVelocity(GetActorRightVector() * -1 * DeltaTime * moveSpeed);

		AddActorLocalOffset(GetActorRightVector() * -1 * DeltaTime * moveSpeed);
		//Pad->AddForce(GetActorRightVector() * -1 * DeltaTime * moveSpeed, NAME_None, true);
		//SetActorLocation(GetActorLocation() + GetActorRightVector() * -1 * DeltaTime * moveSpeed, true);
		//if (playerPtr)
			//playerPtr->AddActorLocalOffset(GetActorRightVector() * -1 * DeltaTime * moveSpeed);

			//playerPtr->SetActorLocation(playerPtr->GetActorLocation() + GetActorRightVector() * -1 * DeltaTime * moveSpeed, true);
			//playerPtr->mMesh->AddForce(GetActorRightVector() * -1 * DeltaTime * moveSpeed, NAME_None, true);
	}

	if (rightPushed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Adding offset right"));
		//Pad->SetPhysicsLinearVelocity(GetActorRightVector() * DeltaTime * moveSpeed);
		//if (playerPtr)
			//playerPtr->mMesh->SetPhysicsLinearVelocity(GetActorRightVector() * DeltaTime * moveSpeed);

		AddActorLocalOffset(GetActorRightVector() * DeltaTime * moveSpeed);
		//Pad->AddForce(GetActorRightVector() * DeltaTime * moveSpeed, NAME_None, true);
		//SetActorLocation(GetActorLocation() + GetActorRightVector() * DeltaTime * moveSpeed, true);
		//if (playerPtr)
			//playerPtr->AddActorLocalOffset(GetActorRightVector() * DeltaTime * moveSpeed);

			//playerPtr->SetActorLocation(playerPtr->GetActorLocation() + GetActorRightVector() * DeltaTime * moveSpeed, true);
			//playerPtr->mMesh->AddForce(GetActorRightVector() * DeltaTime * moveSpeed, NAME_None, true);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Up: %s, Down: %s, Left: %s, Right: %s"), upPushed ? TEXT("true") : TEXT("false"), downPushed ? TEXT("true") : TEXT("false"), leftPushed ? TEXT("true") : TEXT("false"), rightPushed ? TEXT("true") : TEXT("false"));

}

void AMoveablePlatform::OnOverlap(UPrimitiveComponent * OverlappedComponent, 
	AActor * OtherActor, UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		playerPtr = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0));
		//playerPtr->onMoveablePlatform = true;

		//playerPtr->mMesh->SetLinearDamping(0);
		//playerPtr->mMesh->SetAngularDamping(0);

		//playerPtr->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		if (OverlappedComponent->GetName().Compare("Up") == 0)
			upPushed = true;

		if (OverlappedComponent->GetName().Compare("Down") == 0)
			downPushed = true;

		if (OverlappedComponent->GetName().Compare("Left") == 0)
			leftPushed = true;

		if (OverlappedComponent->GetName().Compare("Right") == 0)
			rightPushed = true;

		//UE_LOG(LogTemp, Warning, TEXT("%s"), *OverlappedComponent->GetName());
	}


	//OBSTACLES NEED TO GENERATE OVERLAP EVENTS
	if (OtherActor->GetName().Contains("Obstacle", ESearchCase::IgnoreCase))
	{
		//upPushed = false;
		//downPushed = false;
		//leftPushed = false;
		//rightPushed = false;

		Pad->SetSimulatePhysics(true);
		Pad->AddImpulse(FVector(0, 0, -300));

		UE_LOG(LogTemp, Warning, TEXT("Colliding with obstacle"));
	}


}

void AMoveablePlatform::OnEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{

	if (OtherActor->IsA(AGolfBall::StaticClass()))
	{
		playerPtr = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0));

		if (OverlappedComponent->GetName().Compare("Up") == 0)
			upPushed = false;

		if (OverlappedComponent->GetName().Compare("Down") == 0)
			downPushed = false;

		if (OverlappedComponent->GetName().Compare("Left") == 0)
			leftPushed = false;

		if (OverlappedComponent->GetName().Compare("Right") == 0)
			rightPushed = false;
	}
	//playerPtr->onMoveablePlatform = true;
	//playerPtr->mMesh->SetLinearDamping(100.f);
	//	playerPtr->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AMoveablePlatform::OnOverlapUp(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{


	if (OverlappedComponent->ComponentHasTag("Up"))
	{
	upPushed = true;
	//downPushed = false;
	//leftPushed = false;
	//rightPushed = false;
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapUp"));

	playerPtr = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0));
	}

}

void AMoveablePlatform::OnEndOverlapUp(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{

	//Pad->SetPhysicsLinearVelocity(FVector::ZeroVector);

	if (OverlappedComponent->ComponentHasTag("Up"))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnOverlapEndUp"));

		upPushed = false;

	}
}

void AMoveablePlatform::OnOverlapDown(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OverlappedComponent->ComponentHasTag("Down"))
	{
		UE_LOG(LogTemp, Warning, TEXT("OnOverlapDown"));

	//upPushed = false;
	downPushed = true;
	//leftPushed = false;
	//rightPushed = false;
	playerPtr = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0));
	}

}

void AMoveablePlatform::OnEndOverlapDown(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{

	//Pad->SetPhysicsLinearVelocity(FVector::ZeroVector);
	if (OverlappedComponent->ComponentHasTag("Down"))
	{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapEndDown"));
	downPushed = false;

	}
}

void AMoveablePlatform::OnOverlapLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if (OverlappedComponent->ComponentHasTag("Left"))
	{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapLeft"));
	//upPushed = false;
	//downPushed = false;
	leftPushed = true;
	//rightPushed = false;

	playerPtr = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0));
	}


}

void AMoveablePlatform::OnEndOverlapLeft(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{

	if (OverlappedComponent->ComponentHasTag("Left"))
	{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapEndLeft"));

	//Pad->SetPhysicsLinearVelocity(FVector::ZeroVector);

	leftPushed = false;
	}

}

void AMoveablePlatform::OnOverlapRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor,
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OverlappedComponent->ComponentHasTag("Right"))
	{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapRight"));

	//upPushed = false;
	//downPushed = false;
	//leftPushed = false;
	rightPushed = true;

	playerPtr = Cast<AGolfBall>(UGameplayStatics::GetPlayerPawn(this, 0));
	}

}

void AMoveablePlatform::OnEndOverlapRight(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, 
	UPrimitiveComponent * OtherComponent, int32 OtherBodyIndex)
{
	if (OverlappedComponent->ComponentHasTag("Right"))
	{
	UE_LOG(LogTemp, Warning, TEXT("OnOverlapEndRight"));

	//Pad->SetPhysicsLinearVelocity(FVector::ZeroVector);

	rightPushed = false;
	}

}
