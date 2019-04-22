// Fill out your copyright notice in the Description page of Project Settings.

#include "PatternObstacle.h"

// Sets default values
APatternObstacle::APatternObstacle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatternObstacle::BeginPlay()
{
	Super::BeginPlay();

	initialLocation = GetActorLocation();
}

// Called every frame
void APatternObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!active)
	{
		delayTimer += DeltaTime;
		if (delayTimer >= delay)
			active = true;
	}

	if (active)
	{
		if (pattern1)
		{
			prevLocation1 = GetActorLocation();

			if (pattern1M1)
			{
				AddActorLocalOffset(GetActorForwardVector() * DeltaTime * pattern1Speed);

				pattern1RangeCounter += (prevLocation1 - GetActorLocation()).Size();

				if (pattern1RangeCounter >= pattern1Range)
				{
					pattern1M1 = false;
					pattern1M2 = true;
					pattern1RangeCounter = 0.f;
				}
			}

			if (pattern1M2)
			{
				AddActorLocalOffset(GetActorUpVector() * -1 * DeltaTime * pattern1Speed);

				pattern1RangeCounter += (prevLocation1 - GetActorLocation()).Size();

				if (pattern1RangeCounter >= pattern1Range)
				{
					pattern1M2 = false;
					pattern1M3 = true;
					pattern1RangeCounter = 0.f;
				}
			}

			if (pattern1M3)
			{
				AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * pattern1Speed);

				pattern1RangeCounter += (prevLocation1 - GetActorLocation()).Size();

				if (pattern1RangeCounter >= pattern1Range)
				{
					pattern1M3 = false;
					pattern1M4 = true;
					pattern1RangeCounter = 0.f;
				}
			}

			if (pattern1M4)
			{
				AddActorLocalOffset(GetActorUpVector() * DeltaTime * pattern1Speed);

				pattern1RangeCounter += (prevLocation1 - GetActorLocation()).Size();

				if (pattern1RangeCounter >= pattern1Range)
				{
					pattern1M4 = false;
					pattern1M1 = true;
					pattern1RangeCounter = 0.f;
				}
			}

		}

		if (pattern2)
		{
			prevLocation2 = GetActorLocation();

			if (pattern2M1)
			{
				AddActorLocalOffset(GetActorUpVector() * -1 * DeltaTime * pattern2Speed);

				pattern2RangeCounter += (prevLocation2 - GetActorLocation()).Size();

				if (pattern2RangeCounter >= pattern2Range)
				{
					pattern2M1 = false;
					pattern2M2 = true;
					pattern2RangeCounter = 0.f;
				}
			}

			if (pattern2M2)
			{
				AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * pattern2Speed);

				pattern2RangeCounter += (prevLocation2 - GetActorLocation()).Size();

				if (pattern2RangeCounter >= pattern2Range)
				{
					pattern2M2 = false;
					pattern2M3 = true;
					pattern2RangeCounter = 0.f;
				}
			}

			if (pattern2M3)
			{
				AddActorLocalOffset(GetActorUpVector() * DeltaTime * pattern2Speed);

				pattern2RangeCounter += (prevLocation2 - GetActorLocation()).Size();

				if (pattern2RangeCounter >= pattern2Range)
				{
					pattern2M3 = false;
					pattern2M4 = true;
					pattern2RangeCounter = 0.f;
				}
			}

			if (pattern2M4)
			{
				AddActorLocalOffset(GetActorForwardVector() * DeltaTime * pattern2Speed);

				pattern2RangeCounter += (prevLocation2 - GetActorLocation()).Size();

				if (pattern2RangeCounter >= pattern2Range)
				{
					pattern2M4 = false;
					pattern2M1 = true;
					pattern2RangeCounter = 0.f;
				}
			}

		}

		if (pattern3)
		{
			prevLocation3 = GetActorLocation();

			if (pattern3M1)
			{
				AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * pattern3Speed);

				pattern3RangeCounter += (prevLocation3 - GetActorLocation()).Size();

				if (pattern3RangeCounter >= pattern3Range)
				{
					pattern3M1 = false;
					pattern3M2 = true;
					pattern3RangeCounter = 0.f;
				}
			}

			if (pattern3M2)
			{
				AddActorLocalOffset(GetActorUpVector() * DeltaTime * pattern3Speed);

				pattern3RangeCounter += (prevLocation3 - GetActorLocation()).Size();

				if (pattern3RangeCounter >= pattern3Range)
				{
					pattern3M2 = false;
					pattern3M3 = true;
					pattern3RangeCounter = 0.f;
				}
			}

			if (pattern3M3)
			{
				AddActorLocalOffset(GetActorForwardVector() * DeltaTime * pattern3Speed);

				pattern3RangeCounter += (prevLocation3 - GetActorLocation()).Size();

				if (pattern3RangeCounter >= pattern3Range)
				{
					pattern3M3 = false;
					pattern3M4 = true;
					pattern3RangeCounter = 0.f;
				}
			}

			if (pattern3M4)
			{
				AddActorLocalOffset(GetActorUpVector() * -1 * DeltaTime * pattern3Speed);

				pattern3RangeCounter += (prevLocation3 - GetActorLocation()).Size();

				if (pattern3RangeCounter >= pattern3Range)
				{
					pattern3M4 = false;
					pattern3M1 = true;
					pattern3RangeCounter = 0.f;
				}
			}

		}

		if (pattern4)
		{
			prevLocation4 = GetActorLocation();

			if (pattern4M1)
			{
				AddActorLocalOffset(GetActorUpVector() * DeltaTime * pattern4Speed);

				pattern4RangeCounter += (prevLocation4 - GetActorLocation()).Size();

				if (pattern4RangeCounter >= pattern4Range)
				{
					pattern4M1 = false;
					pattern4M2 = true;
					pattern4RangeCounter = 0.f;
				}
			}

			if (pattern4M2)
			{
				AddActorLocalOffset(GetActorForwardVector() * DeltaTime * pattern4Speed);

				pattern4RangeCounter += (prevLocation4 - GetActorLocation()).Size();

				if (pattern4RangeCounter >= pattern4Range)
				{
					pattern4M2 = false;
					pattern4M3 = true;
					pattern4RangeCounter = 0.f;
				}
			}

			if (pattern4M3)
			{
				AddActorLocalOffset(GetActorUpVector() * -1 * DeltaTime * pattern4Speed);

				pattern4RangeCounter += (prevLocation4 - GetActorLocation()).Size();

				if (pattern4RangeCounter >= pattern4Range)
				{
					pattern4M3 = false;
					pattern4M4 = true;
					pattern4RangeCounter = 0.f;
				}
			}

			if (pattern4M4)
			{
				AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * pattern4Speed);

				pattern4RangeCounter += (prevLocation4 - GetActorLocation()).Size();

				if (pattern4RangeCounter >= pattern4Range)
				{
					pattern4M4 = false;
					pattern4M1 = true;
					pattern4RangeCounter = 0.f;
				}
			}
		}

		if (pattern5)
		{
			prevLocation5 = GetActorLocation();

			if (pattern5M1)
			{
				AddActorLocalOffset(GetActorForwardVector() * DeltaTime * pattern5Speed);

				pattern5RangeCounter += (prevLocation5 - GetActorLocation()).Size();

				if (pattern5RangeCounter >= pattern5Range)
				{
					pattern5M1 = false;
					pattern5M2 = true;
					pattern5RangeCounter = 0.f;
				}
			}

			if (pattern5M2)
			{
				AddActorLocalOffset(GetActorRightVector() * DeltaTime * pattern5Speed);

				pattern5RangeCounter += (prevLocation5 - GetActorLocation()).Size();

				if (pattern5RangeCounter >= pattern5Range)
				{
					pattern5M2 = false;
					pattern5M3 = true;
					pattern5RangeCounter = 0.f;
				}
			}

			if (pattern5M3)
			{
				AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * pattern5Speed);

				pattern5RangeCounter += (prevLocation5 - GetActorLocation()).Size();

				if (pattern5RangeCounter >= pattern5Range)
				{
					pattern5M3 = false;
					pattern5M4 = true;
					pattern5RangeCounter = 0.f;
				}
			}

			if (pattern5M4)
			{
				AddActorLocalOffset(GetActorRightVector() * -1 * DeltaTime * pattern5Speed);

				pattern5RangeCounter += (prevLocation5 - GetActorLocation()).Size();

				if (pattern5RangeCounter >= pattern5Range)
				{
					pattern5M4 = false;
					pattern5M1 = true;
					pattern5RangeCounter = 0.f;
				}
			}

		}

		if (pattern6)
		{
			prevLocation6 = GetActorLocation();

			if (pattern6M1)
			{
				AddActorLocalOffset(GetActorForwardVector() * -1 * DeltaTime * pattern6Speed);

				pattern6RangeCounter += (prevLocation6 - GetActorLocation()).Size();

				if (pattern6RangeCounter >= pattern6Range)
				{
					pattern6M1 = false;
					pattern6M2 = true;
					pattern6RangeCounter = 0.f;
				}
			}

			if (pattern6M2)
			{
				AddActorLocalOffset(GetActorRightVector() * -1 * DeltaTime * pattern6Speed);

				pattern6RangeCounter += (prevLocation6 - GetActorLocation()).Size();

				if (pattern6RangeCounter >= pattern6Range)
				{
					pattern6M2 = false;
					pattern6M3 = true;
					pattern6RangeCounter = 0.f;
				}
			}

			if (pattern6M3)
			{
				AddActorLocalOffset(GetActorForwardVector() * DeltaTime * pattern6Speed);

				pattern6RangeCounter += (prevLocation6 - GetActorLocation()).Size();

				if (pattern6RangeCounter >= pattern6Range)
				{
					pattern6M3 = false;
					pattern6M4 = true;
					pattern6RangeCounter = 0.f;
				}
			}

			if (pattern6M4)
			{
				AddActorLocalOffset(GetActorRightVector() * DeltaTime * pattern6Speed);

				pattern6RangeCounter += (prevLocation6 - GetActorLocation()).Size();

				if (pattern6RangeCounter >= pattern6Range)
				{
					pattern6M4 = false;
					pattern6M1 = true;
					pattern6RangeCounter = 0.f;
				}
			}
		}
	}
}

