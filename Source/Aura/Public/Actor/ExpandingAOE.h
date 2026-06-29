// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffect.h"
#include "GameFramework/Actor.h"
#include "ExpandingAOE.generated.h"

UENUM(BlueprintType)
enum class EAOEShape : uint8
{
	FullCircle UMETA(DisplayName = "Full Circle"),
	Cone UMETA(DisplayName = "Cone")
};


UCLASS()
class AURA_API AExpandingAOE : public AActor
{
	GENERATED_BODY()

public:
	AExpandingAOE();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateWave(float DeltaTime);
	void CheckForHits();
	bool IsTargetInsideWave(AActor* Target) const;
	void ApplyHit(AActor* Target);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	EAOEShape Shape = EAOEShape::FullCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	float MaxRadius = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	float WaveSpeed = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	float WaveThickness = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	float ConeAngleDegrees = 90.f;

	UPROPERTY(BlueprintReadWrite, Category = "AOE")
	FVector Origin = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, Category = "AOE")
	FVector WaveForward = FVector::ForwardVector;

	UPROPERTY(BlueprintReadWrite, Category = "AOE")
	TObjectPtr<UAbilitySystemComponent> SourceASC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AOE")
	TSubclassOf<UGameplayEffect> DebuffEffectClass;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

private:
	float ElapsedTime = 0.f;
	float CurrentRadius = 0.f;
	float PreviousRadius = 0.f;

	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;
};
