// Copyright Salmas Corporation

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