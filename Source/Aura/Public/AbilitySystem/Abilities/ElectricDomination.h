// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "ElectricDomination.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectricDomination : public UAuraBeamSpell
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void BindPrimaryTargetDeathEvent();

	UFUNCTION(BlueprintCallable)
	void BindAdditionalTargetDeathEvent(AActor* AdditionalTarget);
	
};
