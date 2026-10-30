// Copyright Salmas Corporation

#include "Engine/Texture2D.h"
#include "Materials/MaterialInterface.h"
#include "AbilitySystem/Data/ProgressionSelectionData.h"

const FProgressionSelectionDefinition* UProgressionSelectionData::FindSelectionDefinition(const FGameplayTag& SelectionTag, bool bLogNotFound) const
{
	for (const FProgressionSelectionDefinition& Definition : SelectionDefinitions)
	{
		if (Definition.SelectionTag.MatchesTagExact(SelectionTag))
		{
			return &Definition;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Selection definition [%s] was not found in [%s]."), *SelectionTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

bool UProgressionSelectionData::GetSelectionDefinitionForTag(const FGameplayTag& SelectionTag, FProgressionSelectionDefinition& OutDefinition) const
{
	const FProgressionSelectionDefinition* Definition = FindSelectionDefinition(SelectionTag, true);

	if (Definition == nullptr)
	{
		OutDefinition = FProgressionSelectionDefinition();
		return false;
	}

	OutDefinition = *Definition;
	return true;
}

const FReactionMasteryRule* UProgressionSelectionData::FindReactionMasteryRule(const FGameplayTag& MasteryTag, bool bLogNotFound) const
{
	for (const FReactionMasteryRule& Rule : ReactionMasteryRules)
	{
		if (Rule.MasteryTag.MatchesTagExact(MasteryTag))
		{
			return &Rule;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Reaction mastery rule not found for mastery [%s] on [%s]."), *MasteryTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

bool UProgressionSelectionData::FindReactionUnlockPresentation(const FGameplayTag& ReactionTag,
	FProgressionRewardPresentation& OutPresentation) const
{
	for (const FProgressionRewardPresentation& Presentation : ReactionUnlockPresentations)
	{
		if (Presentation.RewardTag.MatchesTagExact(ReactionTag))
		{
			OutPresentation = Presentation;
			return true;
		}
	}

	return false;
}
