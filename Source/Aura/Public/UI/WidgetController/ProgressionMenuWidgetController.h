// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/ProgressionSelectionData.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "ProgressionMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProgressionSelectionOpenedSignature, const FProgressionSelectionDefinition&, Definition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProgressionSelectionResultSignature, FGameplayTag, RewardTag, bool, bSuccess);

UCLASS(BlueprintType, Blueprintable)
class AURA_API UProgressionMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbackToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="Progression")
	FProgressionSelectionOpenedSignature ProgressionSelectionOpenedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Progression")
	FProgressionSelectionResultSignature ProgressionSelectionResultDelegate;

	UFUNCTION(BlueprintCallable, Category="Progression")
	bool OpenProgressionSelection(const FGameplayTag& SelectionTag);

	UFUNCTION(BlueprintCallable, Category="Progression")
	void ChooseProgressionReward(const FGameplayTag& RewardTag);

	UFUNCTION(BlueprintPure, Category="Progression")
	bool HasActiveSelection() const;

private:
	UFUNCTION()
	void OnProgressionSelectionResolved(FGameplayTag SelectionTag, FGameplayTag RewardTag, bool bSuccess);

	FGameplayTag ActiveSelectionTag;
};