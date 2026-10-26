// Copyright Salmas Corporation

#include "UI/WidgetController/ProgressionMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UProgressionMenuWidgetController::BindCallbackToDependencies()
{
    if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
    {
        AuraASC->ProgressionSelectionResolvedDelegate.AddUniqueDynamic(this, &UProgressionMenuWidgetController::OnProgressionSelectionResolved);
    }
}

bool UProgressionMenuWidgetController::OpenProgressionSelection(const FGameplayTag& SelectionTag)
{
    UAuraAbilitySystemComponent* AuraASC = GetAuraASC();
    if (!AuraASC)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot open progression selection because AuraAbilitySystemComponent is invalid."));
        return false;
    }

    const UProgressionSelectionData* SelectionData = UAuraAbilitySystemLibrary::GetProgressionSelectionData(this);
    if (!SelectionData)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot open progression selection because ProgressionSelectionData was not found."));
        return false;
    }

    const FProgressionSelectionDefinition* Definition = SelectionData->FindSelectionDefinition(SelectionTag, true);
    if (!Definition)
    {
        return false;
    }

    if (!Definition->CompletionTag.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Progression selection [%s] has no valid completion tag."), *SelectionTag.ToString());
        return false;
    }

    if (AuraASC->HasMatchingGameplayTag(Definition->CompletionTag))
    {
        UE_LOG(LogTemp, Warning, TEXT("Progression selection [%s] has already been completed."), *SelectionTag.ToString());
        return false;
    }

    FProgressionSelectionDefinition FilteredDefinition = *Definition;

    FilteredDefinition.Options.RemoveAll(
        [AuraASC](const FProgressionChoiceOption& Option)
        {
            return !AuraASC->MeetsProgressionRequirements(Option.RequiredOwnedAbilityTags, Option.RequiredProgressionTags);
        });

    if (FilteredDefinition.Options.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Progression selection [%s] has no eligible options."), *SelectionTag.ToString());
        return false;
    }

    ActiveSelectionTag = SelectionTag;
    ProgressionSelectionOpenedDelegate.Broadcast(FilteredDefinition);
    return true;
}

void UProgressionMenuWidgetController::ChooseProgressionReward(const FGameplayTag& RewardTag)
{
    if (!ActiveSelectionTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ChooseProgressionReward was called without an active progression selection."));
        return;
    }

    if (!RewardTag.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("ChooseProgressionReward received an invalid reward tag."));
        return;
    }

    if (UAuraAbilitySystemComponent* AuraASC = GetAuraASC())
    {
        AuraASC->SubmitProgressionSelection(ActiveSelectionTag, RewardTag);
    }
}

bool UProgressionMenuWidgetController::HasActiveSelection() const
{
    return ActiveSelectionTag.IsValid();
}

void UProgressionMenuWidgetController::OnProgressionSelectionResolved(FGameplayTag SelectionTag, FGameplayTag RewardTag, bool bSuccess)
{
    if (!SelectionTag.MatchesTagExact(ActiveSelectionTag))
    {
        return;
    }

    ProgressionSelectionResultDelegate.Broadcast(RewardTag, bSuccess);

    if (bSuccess)
    {
        ActiveSelectionTag = FGameplayTag();
    }
}