// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual FString GetDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual FString GetNextLevelDescription(int32 Level) override;
};
