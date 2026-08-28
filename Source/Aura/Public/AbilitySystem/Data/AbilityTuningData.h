#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ScalableFloat.h"
#include "AbilityTuningData.generated.h"

USTRUCT(BlueprintType)
struct FAbilityTuningRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Core")
	FScalableFloat Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Core")
	FScalableFloat ManaCost;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Core")
	FScalableFloat Cooldown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procs")
	FScalableFloat DebuffChance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procs")
	FScalableFloat KnockbackChance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procs")
	FScalableFloat StunChance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Procs")
	FScalableFloat SlowChance;
};