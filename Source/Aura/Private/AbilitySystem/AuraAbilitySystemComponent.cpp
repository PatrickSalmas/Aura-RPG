// Copyright Salmas Corporation


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interaction/PlayerInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/ProgressionSelectionData.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			// Below line is deprecated. Using recommended GetDynamicSpecSourceTags() instead
			// AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterSystemAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupSystemAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupSystemAbilities)
	{
		if (!AbilityClass)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		GiveAbility(AbilitySpec);
	}
}
void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.GetAbilityInstances().Last()->GetCurrentActivationInfoRef().GetActivationPredictionKey());
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.GetAbilityInstances().Last()->GetCurrentActivationInfoRef().GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.GetDynamicSpecSourceTags())
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

bool UAuraAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(AbilitySpec, Slot))
		{
			return false;
		}
	}
	return true;
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(Slot);
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec& Spec)
{
	return Spec.GetDynamicSpecSourceTags().HasTag(FGameplayTag::RequestGameplayTag(FName("InputTag")));
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(Slot))
		{
			return &AbilitySpec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& Spec)
{
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	const FGameplayTag AbilityTag = GetAbilityTagFromSpec(Spec);
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const FGameplayTag AbilityType = Info.AbilityType;
	return AbilityType.MatchesTag(FAuraGameplayTags::Get().Abilities_Type_Passive);
}

void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.GetDynamicSpecSourceTags().AddTag(Slot);
}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivatePassiveEffectDelegate.Broadcast(AbilityTag, bActivate);
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributesPoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.GetDynamicSpecSourceTags().AddTag(FAuraGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus_Implementation(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_Status_Eligible, 1);
		}
	}
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
		{
			AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(GameplayTags.Abilities_Status_Eligible);
			AbilitySpec->GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);
			Status = GameplayTags.Abilities_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped) || Status.MatchesTagExact(
			GameplayTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		ClientUpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == GameplayTags.Abilities_Status_Equipped || Status ==
			GameplayTags.Abilities_Status_Unlocked;
		if (bStatusValid)
		{
			// Handle activation/deactivation for passive abilities

			if (!SlotIsEmpty(Slot)) // There is an ability in this slot already. Deactivate and clear its slot
			{
				FGameplayAbilitySpec* SpecWithSlot = GetSpecWithSlot(Slot);
				if (SpecWithSlot)
				{
					// Is that ability the same as this ability? If so, return early
					if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
					{
						ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
						return;
					}

					if (IsPassiveAbility(*SpecWithSlot))
					{
						MulticastActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false);
						DeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
					}

					ClearSlot(SpecWithSlot);
				}
			}
			
			if (!AbilityHasAnySlot(*AbilitySpec)) // Ability doesn't yet have a slot (it's not active)
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MulticastActivatePassiveEffect(AbilityTag, true);
				}
			}
			AssignSlotToAbility(*AbilitySpec, Slot);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, Slot, PrevSlot);
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquippedDelegate.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
                                                              FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None))
	{
		OutDescription = FString();
	}
	OutDescription = UAuraDamageGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	OutNextLevelDescription = FString();
	return false;
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->GetDynamicSpecSourceTags().RemoveTag(Slot);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, SlotTag))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* AbilitySpec, const FGameplayTag& SlotTag)
{
	for (FGameplayTag Tag : AbilitySpec->GetDynamicSpecSourceTags())
	{
		if (Tag.MatchesTagExact(SlotTag))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::UnlockProgressionTag(const FGameplayTag& UnlockTag)
{
	if (!UnlockTag.IsValid())
	{
		return;
	}

	if (IsOwnerActorAuthoritative())
	{
		GrantProgressionUnlock(UnlockTag);
	}
	else
	{
		ServerUnlockProgressionTag(UnlockTag);
	}
}


void UAuraAbilitySystemComponent::ServerUnlockProgressionTag_Implementation(const FGameplayTag& UnlockTag)
{
	GrantProgressionUnlock(UnlockTag);
}

bool UAuraAbilitySystemComponent::HasProgressionUnlock(const FGameplayTag& UnlockTag) const
{
	return UnlockTag.IsValid() && HasMatchingGameplayTag(UnlockTag);
}

void UAuraAbilitySystemComponent::SubmitProgressionSelection(const FGameplayTag& SelectionTag, const FGameplayTag& RewardTag)
{
	if (!SelectionTag.IsValid() || !RewardTag.IsValid())
	{
		ProgressionSelectionResolvedDelegate.Broadcast(SelectionTag, RewardTag, false);
		return;
	}

	if (GetOwner() && GetOwner()->HasAuthority())
	{
		const bool bSuccess = CommitProgressionSelection(SelectionTag, RewardTag);
		ClientProgressionSelectionResolved(SelectionTag, RewardTag, bSuccess);
		return;
	}

	ServerSubmitProgressionSelection(SelectionTag, RewardTag);
}

void UAuraAbilitySystemComponent::ServerSubmitProgressionSelection_Implementation(const FGameplayTag& SelectionTag, const FGameplayTag& RewardTag)
{
	const bool bSuccess = CommitProgressionSelection(SelectionTag, RewardTag);
	ClientProgressionSelectionResolved(SelectionTag, RewardTag, bSuccess);
}

void UAuraAbilitySystemComponent::ClientProgressionSelectionResolved_Implementation(const FGameplayTag& SelectionTag, const FGameplayTag& RewardTag, bool bSuccess)
{
	ProgressionSelectionResolvedDelegate.Broadcast(SelectionTag, RewardTag, bSuccess);
}

bool UAuraAbilitySystemComponent::IsValidProgressionUnlock(const FGameplayTag& UnlockTag) const
{
	if (!UnlockTag.IsValid())
	{
		return false;
	}

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	const bool bIsMasteryUnlock = GameplayTags.ReactiveStatusesToMasteries.FindKey(UnlockTag) != nullptr;
	const bool bIsReactionUnlock = GameplayTags.ReactionsToUnlocks.FindKey(UnlockTag) != nullptr;

	return bIsMasteryUnlock || bIsReactionUnlock;
}

void UAuraAbilitySystemComponent::GrantProgressionUnlock(const FGameplayTag& UnlockTag)
{
	if (!IsOwnerActorAuthoritative())
	{
		return;
	}

	if (!IsValidProgressionUnlock(UnlockTag))
	{
		UE_LOG(LogTemp, Error, TEXT("GrantProgressionUnlock: %s is not a recognized progression unlock."), *UnlockTag.ToString());
		return;
	}

	if (HasMatchingGameplayTag(UnlockTag))
	{
		return;
	}

	AddLooseGameplayTag(UnlockTag);

	UE_LOG(LogTemp, Log, TEXT("Granted progression unlock: %s"), *UnlockTag.ToString());
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}

bool UAuraAbilitySystemComponent::GrantAbilityReward(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag)
{
    if (!IsOwnerActorAuthoritative())
    {
        UE_LOG(LogTemp, Error, TEXT("GrantAbilityReward must be called by the authority."));
        return false;
    }

    if (!AbilityTag.IsValid() || !SlotTag.IsValid())
    {
        return false;
    }

    const FGameplayTag AbilitySlotRoot = FGameplayTag::RequestGameplayTag(FName("InputTag.Ability"));
    if (!SlotTag.MatchesTag(AbilitySlotRoot))
    {
        UE_LOG(LogTemp, Error, TEXT("GrantAbilityReward: %s is not a semantic ability slot."), *SlotTag.ToString());
        return false;
    }

    FGameplayAbilitySpec* ExistingSpec = GetSpecFromAbilityTag(AbilityTag);

    if (ExistingSpec && AbilityHasSlot(*ExistingSpec, SlotTag))
    {
        return true;
    }

    if (ExistingSpec && AbilityHasAnySlot(*ExistingSpec))
    {
        UE_LOG(LogTemp, Warning, TEXT("GrantAbilityReward: %s is already assigned to another slot."), *AbilityTag.ToString());
        return false;
    }

    if (!SlotIsEmpty(SlotTag))
    {
        UE_LOG(LogTemp, Warning, TEXT("GrantAbilityReward: %s is already occupied."), *SlotTag.ToString());
        return false;
    }

    FGameplayAbilitySpec* AbilitySpec = UnlockAbilityInternal(AbilityTag);
    if (!AbilitySpec)
    {
        return false;
    }

    return EquipAbilityToEmptySlotInternal(*AbilitySpec, SlotTag);
}

bool UAuraAbilitySystemComponent::HasOwnedAbilityMatchingTag(const FGameplayTag& RequiredAbilityTag)
{
	if (!RequiredAbilityTag.IsValid())
	{
		return false;
	}

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		const FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
		if (!AbilityTag.IsValid() || !AbilityTag.MatchesTag(RequiredAbilityTag))
		{
			continue;
		}

		const FGameplayTag Status = GetStatusFromSpec(AbilitySpec);
		const bool bOwned = Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) || Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped);

		if (bOwned)
		{
			return true;
		}
	}

	return false;
}

bool UAuraAbilitySystemComponent::MeetsProgressionRequirements(const FGameplayTagContainer& RequiredOwnedAbilityTags, const FGameplayTagContainer& RequiredProgressionTags)
{
	for (const FGameplayTag& RequiredAbilityTag : RequiredOwnedAbilityTags)
	{
		if (!HasOwnedAbilityMatchingTag(RequiredAbilityTag))
		{
			return false;
		}
	}

	for (const FGameplayTag& RequiredProgressionTag : RequiredProgressionTags)
	{
		if (!HasProgressionUnlock(RequiredProgressionTag))
		{
			return false;
		}
	}

	return true;
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::UnlockAbilityInternal(const FGameplayTag& AbilityTag)
{
    if (!IsOwnerActorAuthoritative() || !AbilityTag.IsValid())
    {
        return nullptr;
    }

    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

    if (FGameplayAbilitySpec* ExistingSpec = GetSpecFromAbilityTag(AbilityTag))
    {
        const FGameplayTag ExistingStatus = GetStatusFromSpec(*ExistingSpec);

        if (!ExistingStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) && !ExistingStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
        {
            SetAbilityStatusInternal(*ExistingSpec, GameplayTags.Abilities_Status_Unlocked);
            ClientUpdateAbilityStatus(AbilityTag, GameplayTags.Abilities_Status_Unlocked, ExistingSpec->Level);
        }

        return ExistingSpec;
    }

    UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
    if (!AbilityInfo)
    {
        UE_LOG(LogTemp, Error, TEXT("UnlockAbilityInternal: AbilityInfo could not be found."));
        return nullptr;
    }

    const FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag, true);
    if (!Info.AbilityTag.IsValid() || !Info.Ability)
    {
        UE_LOG(LogTemp, Error, TEXT("UnlockAbilityInternal: No ability class was found for %s."), *AbilityTag.ToString());
        return nullptr;
    }

    FGameplayAbilitySpec NewAbilitySpec(Info.Ability, 1);
    NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(GameplayTags.Abilities_Status_Unlocked);

    const FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(NewAbilitySpec);
    FGameplayAbilitySpec* GrantedSpec = FindAbilitySpecFromHandle(AbilityHandle);

    if (!GrantedSpec)
    {
        UE_LOG(LogTemp, Error, TEXT("UnlockAbilityInternal: Failed to grant %s."), *AbilityTag.ToString());
        return nullptr;
    }

    MarkAbilitySpecDirty(*GrantedSpec);
    ClientUpdateAbilityStatus(AbilityTag, GameplayTags.Abilities_Status_Unlocked, GrantedSpec->Level);

    return GrantedSpec;
}


bool UAuraAbilitySystemComponent::EquipAbilityToEmptySlotInternal(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag)
{
    if (!IsOwnerActorAuthoritative() || !SlotTag.IsValid())
    {
        return false;
    }

    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
    const FGameplayTag AbilityTag = GetAbilityTagFromSpec(AbilitySpec);

    if (!AbilityTag.IsValid())
    {
        return false;
    }

    if (AbilityHasSlot(AbilitySpec, SlotTag))
    {
        return true;
    }

    if (AbilityHasAnySlot(AbilitySpec))
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipAbilityToEmptySlotInternal: %s already occupies another slot."), *AbilityTag.ToString());
        return false;
    }

    if (!SlotIsEmpty(SlotTag))
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipAbilityToEmptySlotInternal: %s is already occupied."), *SlotTag.ToString());
        return false;
    }

    const FGameplayTag Status = GetStatusFromSpec(AbilitySpec);
    if (!Status.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked) && !Status.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipAbilityToEmptySlotInternal: %s is not unlocked."), *AbilityTag.ToString());
        return false;
    }

    AbilitySpec.GetDynamicSpecSourceTags().AddTag(SlotTag);
    SetAbilityStatusInternal(AbilitySpec, GameplayTags.Abilities_Status_Equipped);

    ClientUpdateAbilityStatus(AbilityTag, GameplayTags.Abilities_Status_Equipped, AbilitySpec.Level);
    ClientEquipAbility(AbilityTag, GameplayTags.Abilities_Status_Equipped, SlotTag, FGameplayTag());

    return true;
}

void UAuraAbilitySystemComponent::SetAbilityStatusInternal(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& NewStatus)
{
    if (!NewStatus.IsValid())
    {
        return;
    }

    const FGameplayTag StatusRoot = FGameplayTag::RequestGameplayTag(FName("Abilities.Status"));
    FGameplayTagContainer StatusTagsToRemove;

    for (const FGameplayTag& Tag : AbilitySpec.GetDynamicSpecSourceTags())
    {
        if (Tag.MatchesTag(StatusRoot))
        {
            StatusTagsToRemove.AddTag(Tag);
        }
    }

    AbilitySpec.GetDynamicSpecSourceTags().RemoveTags(StatusTagsToRemove);
    AbilitySpec.GetDynamicSpecSourceTags().AddTag(NewStatus);
    MarkAbilitySpecDirty(AbilitySpec);
}

bool UAuraAbilitySystemComponent::CommitProgressionSelection(const FGameplayTag& SelectionTag, const FGameplayTag& RewardTag)
{
    if (!GetOwner() || !GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("CommitProgressionSelection must run on the server."));
        return false;
    }

    const UProgressionSelectionData* SelectionData = UAuraAbilitySystemLibrary::GetProgressionSelectionData(this);
    if (!SelectionData)
    {
        UE_LOG(LogTemp, Error, TEXT("ProgressionSelectionData could not be found."));
        return false;
    }

    const FProgressionSelectionDefinition* Definition = SelectionData->FindSelectionDefinition(SelectionTag, true);
    if (!Definition)
    {
        return false;
    }

    if (!Definition->CompletionTag.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Selection [%s] does not have a valid CompletionTag."), *SelectionTag.ToString());
        return false;
    }

    if (HasMatchingGameplayTag(Definition->CompletionTag))
    {
        UE_LOG(LogTemp, Warning, TEXT("Selection [%s] has already been completed."), *SelectionTag.ToString());
        return false;
    }

    const FProgressionChoiceOption* SelectedOption = Definition->Options.FindByPredicate(
        [&RewardTag](const FProgressionChoiceOption& Option)
        {
            return Option.RewardTag.MatchesTagExact(RewardTag);
        });

    if (!SelectedOption)
    {
        UE_LOG(LogTemp, Warning, TEXT("Reward [%s] does not belong to selection [%s]."), *RewardTag.ToString(), *SelectionTag.ToString());
        return false;
    }

    if (!MeetsProgressionRequirements(SelectedOption->RequiredOwnedAbilityTags, SelectedOption->RequiredProgressionTags))
    {
        UE_LOG(LogTemp, Warning, TEXT("Requirements were not met for reward [%s]."), *RewardTag.ToString());
        return false;
    }

    bool bRewardGranted = false;

    switch (Definition->RewardType)
    {
    case EProgressionRewardType::Ability:
        bRewardGranted = GrantAbilityReward(SelectedOption->RewardTag, Definition->DestinationSlot);
        break;

    case EProgressionRewardType::ProgressionUnlock:
        GrantProgressionUnlock(SelectedOption->RewardTag);
        bRewardGranted = HasProgressionUnlock(SelectedOption->RewardTag);
        break;

    default:
        UE_LOG(LogTemp, Error, TEXT("Selection [%s] has an unsupported reward type."), *SelectionTag.ToString());
        return false;
    }

    if (!bRewardGranted)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to grant reward [%s] for selection [%s]."), *RewardTag.ToString(), *SelectionTag.ToString());
        return false;
    }

    AddLooseGameplayTag(Definition->CompletionTag);

    UE_LOG(LogTemp, Log, TEXT("Completed selection [%s] with reward [%s]."), *SelectionTag.ToString(), *RewardTag.ToString());
    return true;
}
