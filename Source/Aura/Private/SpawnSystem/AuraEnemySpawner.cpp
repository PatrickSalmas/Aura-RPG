#include "SpawnSystem/AuraEnemySpawner.h"
#include "Character/AuraEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

AAuraEnemySpawner::AAuraEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	MaxSpawnGroupSize = FMath::Max(MaxSpawnGroupSize, MinSpawnGroupSize);

	CacheSpawnPoints();

	if (bAutoStart)
	{
		StartSpawning();
	}
}

void AAuraEnemySpawner::StartSpawning()
{
	if (bExhausted || SpawnEntries.Num() == 0)
	{
		return;
	}

	bIsActive = true;
	CurrentSpawnInterval = InitialSpawnInterval;

	ScheduleNextSpawn();
}

void AAuraEnemySpawner::StopSpawning()
{
	bIsActive = false;
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}

bool AAuraEnemySpawner::CanSpawn()
{
	AliveSpawnedEnemies.RemoveAll([](const TObjectPtr<AAuraEnemy>& Enemy)
	{
		return !IsValid(Enemy);
	});

	if (!bIsActive || bExhausted || SpawnEntries.Num() == 0)
	{
		return false;
	}

	if (AliveSpawnedEnemies.Num() >= MaxAliveEnemies)
	{
		return false;
	}

	if (!bInfiniteSpawns && TotalSpawned >= MaxTotalSpawns)
	{
		return false;
	}

	return true;
}

void AAuraEnemySpawner::SpawnEnemy()
{
	if (!CanSpawn())
	{
		if (!bInfiniteSpawns && TotalSpawned >= MaxTotalSpawns)
		{
			bExhausted = true;
			StopSpawning();
			return;
		}

		ScheduleNextSpawn();
		return;
	}

	const int32 DesiredGroupSize = FMath::RandRange(MinSpawnGroupSize, MaxSpawnGroupSize);

	int32 RemainingSpawnCapacity = DesiredGroupSize;

	// Respect MaxAliveEnemies.
	RemainingSpawnCapacity = FMath::Min(RemainingSpawnCapacity, GetAvailableAliveSlots());

	// Respect MaxTotalSpawns unless infinite.
	if (!bInfiniteSpawns)
	{
		const int32 RemainingTotalSpawns = MaxTotalSpawns - TotalSpawned;
		RemainingSpawnCapacity = FMath::Min(RemainingSpawnCapacity, RemainingTotalSpawns);
	}

	for (int32 i = 0; i < RemainingSpawnCapacity; ++i)
	{
		const FVector SpawnLocation = GetSpawnLocation();

		if (!IsSpawnLocationSafe(SpawnLocation))
		{
			continue;
		}

		TSubclassOf<AAuraEnemy> ChosenEnemyClass = ChooseEnemyClass();

		if (!ChosenEnemyClass)
		{
			continue;
		}

		FPendingEnemySpawn PendingSpawn;
		PendingSpawn.EnemyClass = ChosenEnemyClass;
		PendingSpawn.SpawnLocation = SpawnLocation;
		PendingSpawn.SpawnRotation = GetActorRotation();
		PendingSpawnCount++;

		if (bUseSpawnTelegraph && SpawnTelegraphDuration > 0.f)
		{
			if (SpawnTelegraphEffect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					this,
					SpawnTelegraphEffect,
					SpawnLocation,
					FRotator::ZeroRotator
				);
			}

			FTimerHandle PendingSpawnTimerHandle;

			FTimerDelegate SpawnDelegate;
			SpawnDelegate.BindUObject(this, &AAuraEnemySpawner::ExecutePendingSpawn, PendingSpawn);

			GetWorldTimerManager().SetTimer(
				PendingSpawnTimerHandle,
				SpawnDelegate,
				SpawnTelegraphDuration,
				false
			);
		}
		else
		{
			ExecutePendingSpawn(PendingSpawn);
		}
	}

	if (!bInfiniteSpawns && TotalSpawned >= MaxTotalSpawns)
	{
		bExhausted = true;
		StopSpawning();
		return;
	}

	UpdateSpawnInterval();
	ScheduleNextSpawn();
}

void AAuraEnemySpawner::ExecutePendingSpawn(FPendingEnemySpawn PendingSpawn)
{
	PendingSpawnCount = FMath::Max(0, PendingSpawnCount - 1);
	if (!IsValid(this))
	{
		return;
	}

	if (!CanSpawn())
	{
		return;
	}

	if (!IsSpawnLocationSafe(PendingSpawn.SpawnLocation))
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	AAuraEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AAuraEnemy>(
		PendingSpawn.EnemyClass,
		PendingSpawn.SpawnLocation,
		PendingSpawn.SpawnRotation,
		SpawnParams
	);

	if (SpawnedEnemy)
	{
		AliveSpawnedEnemies.Add(SpawnedEnemy);
		SpawnedEnemy->OnDestroyed.AddDynamic(this, &AAuraEnemySpawner::OnSpawnedEnemyDestroyed);

		TotalSpawned++;
	}
}

TSubclassOf<AAuraEnemy> AAuraEnemySpawner::ChooseEnemyClass() const
{
	float TotalWeight = 0.f;

	for (const FEnemySpawnEntry& Entry : SpawnEntries)
	{
		if (Entry.EnemyClass && Entry.Weight > 0.f)
		{
			TotalWeight += Entry.Weight;
		}
	}

	if (TotalWeight <= 0.f)
	{
		return nullptr;
	}

	const float Roll = FMath::FRandRange(0.f, TotalWeight);
	float RunningWeight = 0.f;

	for (const FEnemySpawnEntry& Entry : SpawnEntries)
	{
		if (!Entry.EnemyClass || Entry.Weight <= 0.f)
		{
			continue;
		}

		RunningWeight += Entry.Weight;

		if (Roll <= RunningWeight)
		{
			return Entry.EnemyClass;
		}
	}

	return nullptr;
}

FVector AAuraEnemySpawner::GetSpawnLocation() const
{
	if (SpawnLocationMode == ESpawnLocationMode::SpawnPoints && SpawnPoints.Num() > 0)
	{
		TArray<USceneComponent*> ValidSpawnPoints;

		for (USceneComponent* SpawnPoint : SpawnPoints)
		{
			if (IsValid(SpawnPoint))
			{
				ValidSpawnPoints.Add(SpawnPoint);
			}
		}

		if (ValidSpawnPoints.Num() > 0)
		{
			const int32 RandomIndex = FMath::RandRange(0, ValidSpawnPoints.Num() - 1);
			const FVector Location = ValidSpawnPoints[RandomIndex]->GetComponentLocation();

			UE_LOG(LogTemp, Warning, TEXT("Spawning from spawn point: %s at %s"),
				*ValidSpawnPoints[RandomIndex]->GetName(),
				*Location.ToString());

			return Location;
		}
	}

	const FVector2D RandomPoint = FMath::RandPointInCircle(SpawnRadius);
	const FVector FallbackLocation = GetActorLocation() + FVector(RandomPoint.X, RandomPoint.Y, 0.f);

	UE_LOG(LogTemp, Warning, TEXT("Using fallback spawn radius at %s. SpawnPoints Num: %d"),
		*FallbackLocation.ToString(),
		SpawnPoints.Num());

	return FallbackLocation;
}

void AAuraEnemySpawner::OnSpawnedEnemyDestroyed(AActor* DestroyedActor)
{
	AAuraEnemy* DestroyedEnemy = Cast<AAuraEnemy>(DestroyedActor);

	if (!DestroyedEnemy)
	{
		return;
	}

	AliveSpawnedEnemies.Remove(DestroyedEnemy);
}

int32 AAuraEnemySpawner::GetAvailableAliveSlots() const
{
	return FMath::Max(0, MaxAliveEnemies - AliveSpawnedEnemies.Num() - PendingSpawnCount);
}

void AAuraEnemySpawner::CacheSpawnPoints()
{
	SpawnPoints.Empty();

	TArray<USceneComponent*> ChildComponents;
	GetComponents<USceneComponent>(ChildComponents);

	UE_LOG(LogTemp, Warning, TEXT("%s found %d scene components."),
		*GetName(),
		ChildComponents.Num());

	for (USceneComponent* Component : ChildComponents)
	{
		if (!IsValid(Component))
		{
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("Component: %s Has SpawnPoint Tag: %s"),
			*Component->GetName(),
			Component->ComponentHasTag(SpawnPointTag) ? TEXT("TRUE") : TEXT("FALSE"));

		if (Component->ComponentHasTag(SpawnPointTag))
		{
			SpawnPoints.Add(Component);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("%s cached %d spawn points."),
		*GetName(),
		SpawnPoints.Num());
}

bool AAuraEnemySpawner::IsSpawnLocationSafe(const FVector& SpawnLocation) const
{
	const APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (!PlayerPawn)
	{
		return true;
	}

	const float DistanceToPlayer = FVector::Dist2D(SpawnLocation, PlayerPawn->GetActorLocation());

	return DistanceToPlayer >= MinDistanceFromPlayer;
}

void AAuraEnemySpawner::ScheduleNextSpawn()
{
	if (!bIsActive || bExhausted)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AAuraEnemySpawner::SpawnEnemy,
		CurrentSpawnInterval,
		false
	);
}

void AAuraEnemySpawner::UpdateSpawnInterval()
{
	if (!bDecreaseSpawnIntervalOverTime)
	{
		return;
	}

	CurrentSpawnInterval = FMath::Max(
		MinimumSpawnInterval,
		CurrentSpawnInterval - SpawnIntervalDecreasePerSpawn
	);
}