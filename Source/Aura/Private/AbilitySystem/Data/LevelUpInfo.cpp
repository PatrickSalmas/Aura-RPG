// Copyright Salmas Corporation


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	int32 highestXPRange = XP;
	for (int32 i = 1; i < LevelUpInformation.Num(); i++)
	{
		if (LevelUpInformation.Num() <= Level) return Level;
		if (XP >= LevelUpInformation[i].LevelUpRequirement)
		{
			Level = i + 1;
			highestXPRange = LevelUpInformation[i].LevelUpRequirement;
		}
		else
		{
			return Level;
		}
	}
	
	return Level;
}
