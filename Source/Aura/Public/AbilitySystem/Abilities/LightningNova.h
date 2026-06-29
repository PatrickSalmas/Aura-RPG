// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "Actor/ExpandingAOE.h"
#include "LightningNova.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULightningNova : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SpawnLightningNovaWave();
	
	UPROPERTY(EditDefaultsOnly, Category = "AOE")
	TSubclassOf<AExpandingAOE> ExpandingAOEClass;
	
};
