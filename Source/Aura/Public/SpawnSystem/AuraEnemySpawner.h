#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEnemySpawner.generated.h"


class AAuraEnemy;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FEnemySpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="0.0"))
	float Weight = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin="1"))
	int32 SpawnCost = 1;
};

USTRUCT()
struct FPendingEnemySpawn
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY()
	FVector SpawnLocation = FVector::ZeroVector;

	UPROPERTY()
	FRotator SpawnRotation = FRotator::ZeroRotator;
};

UENUM(BlueprintType)
enum class ESpawnLocationMode : uint8
{
	ActorLocationRadius,
	SpawnPoints
};

UCLASS()
class AURA_API AAuraEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AAuraEnemySpawner();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Activation")
	bool bAutoStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Enemies")
	TArray<FEnemySpawnEntry> SpawnEntries;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Timing", meta=(ClampMin="0.1"))
	// float SpawnInterval = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Timing", meta=(ClampMin="0.1"))
	float InitialSpawnInterval = 4.f;
	
	UPROPERTY(BlueprintReadOnly, Category="Spawner|State")
	float CurrentSpawnInterval = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Timing", meta=(ClampMin="0.1"))
	float MinimumSpawnInterval = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Timing")
	bool bDecreaseSpawnIntervalOverTime = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Timing", meta=(EditCondition="bDecreaseSpawnIntervalOverTime", ClampMin="0.0"))
	float SpawnIntervalDecreasePerSpawn = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Limits")
	bool bInfiniteSpawns = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Limits", meta=(EditCondition="!bInfiniteSpawns", ClampMin="1"))
	int32 MaxTotalSpawns = 10;

	UPROPERTY(BlueprintReadOnly, Category="Spawner|State")
	int32 TotalSpawned = 0;

	UPROPERTY(BlueprintReadOnly, Category="Spawner|State")
	bool bIsActive = false;

	UPROPERTY(BlueprintReadOnly, Category="Spawner|State")
	bool bExhausted = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Placement", meta=(ClampMin="0.0"))
	float SpawnRadius = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Limits", meta=(ClampMin="1"))
	int32 MaxAliveEnemies = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Groups", meta=(ClampMin="1"))
	int32 MinSpawnGroupSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Groups", meta=(ClampMin="1"))
	int32 MaxSpawnGroupSize = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Safety", meta=(ClampMin="0.0"))
	float MinDistanceFromPlayer = 500.f;

	UPROPERTY(BlueprintReadOnly, Category="Spawner|State")
	TArray<TObjectPtr<AAuraEnemy>> AliveSpawnedEnemies;
	
	UPROPERTY(BlueprintReadOnly, Category="Spawner|State")
	int32 PendingSpawnCount = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Placement")
	ESpawnLocationMode SpawnLocationMode = ESpawnLocationMode::ActorLocationRadius;

	UPROPERTY(BlueprintReadOnly, Category="Spawner|Placement")
	TArray<TObjectPtr<USceneComponent>> SpawnPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Placement")
	FName SpawnPointTag = FName("SpawnPoint");
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Telegraph")
	bool bUseSpawnTelegraph = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Telegraph", meta=(EditCondition="bUseSpawnTelegraph", ClampMin="0.0"))
	float SpawnTelegraphDuration = 0.75f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spawner|Telegraph", meta=(EditCondition="bUseSpawnTelegraph"))
	TObjectPtr<UNiagaraSystem> SpawnTelegraphEffect;

	UFUNCTION(BlueprintCallable, Category="Spawner")
	void StartSpawning();

	UFUNCTION(BlueprintCallable, Category="Spawner")
	void StopSpawning();
	
	UFUNCTION()
	void OnSpawnedEnemyDestroyed(AActor* DestroyedActor);

private:
	FTimerHandle SpawnTimerHandle;

	void SpawnEnemy();

	bool CanSpawn();
	
	TSubclassOf<AAuraEnemy> ChooseEnemyClass() const;
	
	FVector GetSpawnLocation() const;
	
	int32 GetAvailableAliveSlots() const;
	
	void CacheSpawnPoints();
	
	bool IsSpawnLocationSafe(const FVector& SpawnLocation) const;
	
	void ExecutePendingSpawn(FPendingEnemySpawn PendingSpawn);
	
	void ScheduleNextSpawn();
	
	void UpdateSpawnInterval();
};