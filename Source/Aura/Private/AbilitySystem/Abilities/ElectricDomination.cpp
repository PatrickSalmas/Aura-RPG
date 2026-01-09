// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/ElectricDomination.h"

void UElectricDomination::BindPrimaryTargetDeathEvent()
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathSignatureDelegate().IsAlreadyBound(this, &UAuraBeamSpell::UAuraBeamSpell::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathSignatureDelegate().AddDynamic(this, &UAuraBeamSpell::UAuraBeamSpell::PrimaryTargetDied);
		}
	}
}

void UElectricDomination::BindAdditionalTargetDeathEvent(AActor* AdditionalTarget)
{
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AdditionalTarget))
	{
		if (!CombatInterface->GetOnDeathSignatureDelegate().IsAlreadyBound(this, &UAuraBeamSpell::UAuraBeamSpell::AdditionalTargetDied))
		{
			CombatInterface->GetOnDeathSignatureDelegate().AddDynamic(this, &UAuraBeamSpell::UAuraBeamSpell::AdditionalTargetDied);
		}
	}
}