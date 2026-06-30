// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Actor/ExpandingAOE.h"
#include "FlameFlurry.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UFlameFlurry : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SpawnFlameWave(FDamageEffectParams DamageEffectParams);
	
	UPROPERTY(EditDefaultsOnly, Category = "AOE")
	TSubclassOf<AExpandingAOE> ExpandingAOEClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> FlameFlurryNiagaraSystem;
};
