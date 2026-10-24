// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ProgressionSelectionData.generated.h"

class UMaterialInterface;
class UTexture2D;

UENUM(BlueprintType)
enum class EProgressionRewardType : uint8
{
    Ability,
    ProgressionUnlock
};

USTRUCT(BlueprintType)
struct FProgressionChoiceOption
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag RewardTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText DisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = "true"))
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UTexture2D> Icon = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UMaterialInterface> BackgroundMaterial = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTagContainer RequiredOwnedAbilityTags;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTagContainer RequiredProgressionTags;
};

USTRUCT(BlueprintType)
struct FProgressionSelectionDefinition
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag SelectionTag;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FText Title;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine = "true"))
    FText Prompt;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    EProgressionRewardType RewardType = EProgressionRewardType::Ability;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag DestinationSlot;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    FGameplayTag CompletionTag;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FProgressionChoiceOption> Options;
    
};

UCLASS(BlueprintType)
class AURA_API UProgressionSelectionData : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Progression")
    TArray<FProgressionSelectionDefinition> SelectionDefinitions;

    const FProgressionSelectionDefinition* FindSelectionDefinition(const FGameplayTag& SelectionTag, bool bLogNotFound = false) const;
    
    UFUNCTION(BlueprintPure, Category="Progression")
    bool GetSelectionDefinitionForTag(const FGameplayTag& SelectionTag, FProgressionSelectionDefinition& OutDefinition) const;
};