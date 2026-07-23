// Copyright Salmas Corporation


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
					this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate()->AddDynamic(this, &UDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (NewCount > 0)
	{
		if (IsValid(GetOwner()) && CombatInterface && !CombatInterface->Execute_IsDead(GetOwner()))
		{
			Activate();
			if (CallbackTag.MatchesTagExact(FGameplayTag::RequestGameplayTag("Debuff.Burn")))
			{
				AAuraCharacterBase* TargetCharacter = Cast<AAuraCharacterBase>(GetOwner());
				TargetCharacter->bIsBurned = true;
				TargetCharacter->SetIsBurningEvent(GetOwner());
			}
		}
	}
	else
	{
		Deactivate();
		// if (CallbackTag == GameplayTag)
		if (CallbackTag.MatchesTagExact(FGameplayTag::RequestGameplayTag("Debuff.Burn")))
		{
			AAuraCharacterBase* TargetCharacter = Cast<AAuraCharacterBase>(GetOwner());
			TargetCharacter->bIsBurned = false;
			TargetCharacter->SetIsNotBurningEvent();
		}
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	bool testB = IsActive(); 
	
	Deactivate();
}
