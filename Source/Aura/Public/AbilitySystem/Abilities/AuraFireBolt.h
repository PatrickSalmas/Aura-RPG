// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual FString GetDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	virtual FString GetNextLevelDescription(int32 Level) override;
};
