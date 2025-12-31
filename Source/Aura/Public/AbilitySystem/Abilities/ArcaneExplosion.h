// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "ArcaneExplosion.generated.h"

class AArcaneSplinter;
/**
 * 
 */
UCLASS()
class AURA_API UArcaneExplosion : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable)
	TArray<AArcaneSplinter*> SpawnArcaneSplinters();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ArcaneExplosion")
	int32 NumSplinters = 12;

	// UFUNCTION(BlueprintCallable)
	// GetClosestTarget()
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AArcaneSplinter> ArcaneSplinterClass;
	
};
