// Copyright Salmas Corporation


#include "AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Player/AuraPlayerController.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "UI/Widget/LevelUpTextComponent.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	/* Primary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/* Secondary Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	/* Resistance Attributes */
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetPhysicalResistanceAttribute);
	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	// Vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	// Resistance Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo.IsValid())
	{
		AActor* SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter))
	{
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}
}

void UAuraAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0.f);
	if (LocalIncomingDamage >= 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		if (LocalIncomingDamage > 0.f)
		{
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
		}

		const bool bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			SendXPEvent(Props);
		}
		else if (UAuraAbilitySystemLibrary::GetShouldHitReact(Props.EffectContextHandle))
		{
			if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
		}

			
		const bool bBlock = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		const bool bCriticalHit = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
		ShowFloatingText(Props, LocalIncomingDamage, bBlock, bCriticalHit);
		
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		if (UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle) == GameplayTags.Damage_ArcaneFire)
		{
			bool testVar = false;
		}
		
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			// Handle Debuff
			Debuff(Props);
		}
		
		HandleReaction(Props);
		
		if (UAuraAbilitySystemLibrary::IsSuccessfulReactiveStatus(Props.EffectContextHandle)) 
		{
			ApplyReactiveStatus(Props);
		}
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	if (!Props.SourceASC || !Props.TargetASC)
	{
		return;
	}

	const FAuraGameplayTags& GameplayTags =
		FAuraGameplayTags::Get();

	/*
	 * Retrieve the incoming debuff information from the damage
	 * Gameplay Effect's context.
	 */
	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);

	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);

	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);

	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	const FGameplayTag* DebuffTagPtr = GameplayTags.DamageTypesToDebuffs.Find(DamageType);

	if (!DebuffTagPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Debuff: No debuff tag mapped to damage type %s"), *DamageType.ToString());

		return;
	}

	const FGameplayTag DebuffTag = *DebuffTagPtr;
	
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Knockback))
	{
		Knockback(Props);
		return;
	}
	
	const bool bHasPeriodicDamage =
	DebuffDamage > KINDA_SMALL_NUMBER;

	TArray<FActiveGameplayEffectHandle> EffectsToReplace;

	if (!ShouldApplyPeriodicStatus(Props.TargetASC,DebuffTag,GameplayTags.Debuff_Damage,
			GameplayTags.Debuff_Frequency,DebuffDamage,DebuffFrequency,
								EffectsToReplace))
	{
		return;
	}

	/*
	 * Construct the Gameplay Effect only after inspecting and,
	 * if necessary, removing the existing Burn.
	 */
	UGameplayEffect* Effect = nullptr;

	if (bHasPeriodicDamage)
	{
		/*
		 * Omitting the name causes Unreal to generate a unique
		 * transient UObject name for this Burn definition.
		 */
		Effect = NewObject<UGameplayEffect>(GetTransientPackage());
	}
	else
	{
		/*
		 * Retain the stable definition identity used by the other
		 * debuffs so their AggregateBySource stacking behavior
		 * continues to function as before.
		 */
		const FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"),*DamageType.ToString());

		Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(*DebuffName));
	}

	if (!Effect)
	{
		return;
	}

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;

	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	
	if (bHasPeriodicDamage)
	{
		Effect->Period = DebuffFrequency;

		FGameplayModifierInfo& ModifierInfo = Effect->Modifiers.AddDefaulted_GetRef();

		ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);

		ModifierInfo.ModifierOp = EGameplayModOp::Additive;

		ModifierInfo.Attribute = UAuraAttributeSet::GetIncomingDamageAttribute();
	}

	/*
	 * Grant the debuff tag to the target while this Gameplay Effect
	 * remains active.
	 */
	FInheritedTagContainer InheritedTags;
	InheritedTags.Added.AddTag(DebuffTag);

	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
	{
		InheritedTags.Added.AddTag(GameplayTags.Player_Block_CursorTrace);

		InheritedTags.Added.AddTag(GameplayTags.Player_Block_InputHeld);

		InheritedTags.Added.AddTag(GameplayTags.Player_Block_InputPressed);

		InheritedTags.Added.AddTag(GameplayTags.Player_Block_InputReleased);
	}

	UTargetTagsGameplayEffectComponent& TargetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();

	TargetTagsComponent.SetAndApplyTargetTagChanges(InheritedTags);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;

	Effect->StackLimitCount = 1;
	
	/*
	 * Build the spec on the stack. The ASC copies the spec when it
	 * applies it, so heap allocation is unnecessary.
	 */
	FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();

	EffectContextHandle.AddSourceObject(Props.SourceAvatarActor);

	FGameplayEffectSpec MutableSpec(Effect, EffectContextHandle,1.f);

	/*
	 * Store the debuff values on the individual spec so future
	 * applications can inspect the active Burn's damage and period.
	 */
	MutableSpec.SetSetByCallerMagnitude(GameplayTags.Debuff_Damage, DebuffDamage);

	MutableSpec.SetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, DebuffFrequency);

	MutableSpec.SetSetByCallerMagnitude(GameplayTags.Debuff_Duration, DebuffDuration);

	FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec.GetContext().Get());

	if (AuraContext)
	{
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShared<FGameplayTag>(DamageType);

		AuraContext->SetDamageType(DebuffDamageType);
		AuraContext->SetShouldHitReact(false);
	}

	for (const FActiveGameplayEffectHandle& Handle : EffectsToReplace)
	{
		Props.TargetASC->RemoveActiveGameplayEffect(Handle);
	}

	const FActiveGameplayEffectHandle AppliedHandle = Props.TargetASC->ApplyGameplayEffectSpecToSelf(MutableSpec);

	ensureMsgf(AppliedHandle.IsValid(), TEXT("Failed to apply debuff %s"), *DebuffTag.ToString());
}

void UAuraAttributeSet::ApplyReactiveStatus(
    const FEffectProperties& Props)
{
    if (!Props.SourceASC || !Props.TargetASC)
    {
        return;
    }

    const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

    const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);

    if (!DamageType.IsValid())
    {
        return;
    }

    const FGameplayTag* ReactiveStatusTag = GameplayTags.DamageTypesToReactiveStatuses.Find(DamageType);

    if (!ReactiveStatusTag)
    {
        UE_LOG(LogTemp, Warning, TEXT("No reactive status mapped to %s."), *DamageType.ToString());

        return;
    }

    const UCharacterClassInfo* ClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(Props.TargetAvatarActor);

    if (!ClassInfo)
    {
        return;
    }

    const TSubclassOf<UGameplayEffect>* StatusEffectClass = ClassInfo->ReactiveStatusEffects.Find(*ReactiveStatusTag);

    if (!StatusEffectClass || !StatusEffectClass->Get())
    {
        UE_LOG(LogTemp,Warning, TEXT("No Gameplay Effect configured for reactive status %s."), *ReactiveStatusTag->ToString());

        return;
    }

    FGameplayEffectContextHandle Context = Props.SourceASC->MakeEffectContext();

    Context.AddSourceObject(Props.SourceAvatarActor);

    UAuraAbilitySystemLibrary::SetDamageType(Context, DamageType);

    UAuraAbilitySystemLibrary::SetShouldHitReact(Context,false);

    FGameplayEffectSpecHandle SpecHandle = Props.SourceASC->MakeOutgoingSpec(*StatusEffectClass,1.f,Context);

    if (!SpecHandle.IsValid() || !SpecHandle.Data.IsValid())
    {
        return;
    }

    const float StatusDamage = UAuraAbilitySystemLibrary::GetReactiveStatusDamage(Props.EffectContextHandle);

    const float StatusDuration = UAuraAbilitySystemLibrary::GetReactiveStatusDuration(Props.EffectContextHandle);
	
	const float StatusFrequency = UAuraAbilitySystemLibrary::GetReactiveStatusFrequency(Props.EffectContextHandle);
	
	TArray<FActiveGameplayEffectHandle> EffectsToReplace;

	if (!ShouldApplyPeriodicStatus(Props.TargetASC, *ReactiveStatusTag,GameplayTags.ReactiveStatus_Damage,
			GameplayTags.ReactiveStatus_Frequency,StatusDamage,StatusFrequency,
									EffectsToReplace))
	{
		return;
	}

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.ReactiveStatus_Damage, StatusDamage);

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.ReactiveStatus_Duration, StatusDuration);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.ReactiveStatus_Frequency, StatusFrequency);
	
	SpecHandle.Data->SetDuration(StatusDuration, true);
	SpecHandle.Data->Period = StatusFrequency;

	for (const FActiveGameplayEffectHandle& Handle : EffectsToReplace)
	{
		Props.TargetASC->RemoveActiveGameplayEffect(Handle);
	}

	const FActiveGameplayEffectHandle AppliedHandle = Props.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

	ensureMsgf(AppliedHandle.IsValid(), TEXT("Failed to apply reactive status %s"), *ReactiveStatusTag->ToString());
}

bool UAuraAttributeSet::ShouldApplyPeriodicStatus(UAbilitySystemComponent* TargetASC, const FGameplayTag& StatusTag,
	const FGameplayTag& DamageMagnitudeTag, const FGameplayTag& FrequencyMagnitudeTag, float IncomingDamageVal,
	float IncomingFrequency, TArray<FActiveGameplayEffectHandle>& OutHandlesToReplace) const
{
	 OutHandlesToReplace.Reset();

    if (!TargetASC || !StatusTag.IsValid())
    {
        return false;
    }

    /*
     * This policy only applies to effects that actually deal
     * periodic damage. Non-DOT statuses use their normal stacking
     * configuration.
     */
    if (IncomingDamageVal <= KINDA_SMALL_NUMBER)
    {
        return true;
    }

    if (!FMath::IsFinite(IncomingDamageVal) ||
        !FMath::IsFinite(IncomingFrequency) ||
        IncomingFrequency <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid periodic status values for %s. Damage: %.2f, Frequency: %.2f"),
            *StatusTag.ToString(), IncomingDamageVal, IncomingFrequency);

        return false;
    }

    FGameplayTagContainer StatusTags;
    StatusTags.AddTag(StatusTag);

    const FGameplayEffectQuery StatusQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(StatusTags);

    const TArray<FActiveGameplayEffectHandle> ExistingHandles = TargetASC->GetActiveEffects(StatusQuery);

    if (ExistingHandles.IsEmpty())
    {
        return true;
    }

    const float IncomingDPS = IncomingDamageVal / IncomingFrequency;
    float StrongestExistingDPS = 0.f;

    for (const FActiveGameplayEffectHandle& Handle : ExistingHandles)
    {
        const FActiveGameplayEffect* ActiveEffect = TargetASC->GetActiveGameplayEffect(Handle);

        if (!ActiveEffect)
        {
            continue;
        }

        const float ExistingDamage = ActiveEffect->Spec.GetSetByCallerMagnitude(DamageMagnitudeTag,false,0.f);
        float ExistingFrequency = ActiveEffect->Spec.GetSetByCallerMagnitude(FrequencyMagnitudeTag,false,0.f);

        /*
         * This fallback supports older active effects that set the
         * actual period but did not store frequency as Set-by-Caller.
         */
        if (ExistingFrequency <= KINDA_SMALL_NUMBER)
        {
            ExistingFrequency = ActiveEffect->Spec.GetPeriod();
        }

        const float ExistingDPS = ExistingDamage > KINDA_SMALL_NUMBER && ExistingFrequency > KINDA_SMALL_NUMBER
    							? ExistingDamage / ExistingFrequency: 0.f;

        StrongestExistingDPS =
            FMath::Max(StrongestExistingDPS, ExistingDPS);
    }

    /*
     * Equal and weaker incoming DOTs are rejected and do not refresh
     * the existing effect's duration.
     */
    if (StrongestExistingDPS >= IncomingDPS - KINDA_SMALL_NUMBER)
    {
        return false;
    }

    /*
     * The incoming DOT is stronger. The caller will remove these
     * effects immediately before applying the replacement.
     */
    OutHandlesToReplace = ExistingHandles;

    return true;
}

void UAuraAttributeSet::Knockback(const FEffectProperties& Props)
{
	AAuraCharacterBase* TargetCharacter = Cast<AAuraCharacterBase>(Props.TargetCharacter);
	const FVector KnockbackImpulse = UAuraAbilitySystemLibrary::GetKnockbackImpulse(Props.EffectContextHandle);
	Props.TargetCharacter->GetMovementComponent()->StopMovementImmediately();
	if (!KnockbackImpulse.IsNearlyZero(1.f))
	{
		TargetCharacter->LaunchCharacter(KnockbackImpulse, true, true);
	}
}

void UAuraAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0.f);

	//TODO: See if we should level up

	// Source Character is the owner, since GA_ListenForEvents applies GE_EventBaseEffect, adding to IncomingXP
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps > 0)
		{
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);

			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;

			for (int32 i = 0; i < NumLevelUps; i++)
			{
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter, CurrentLevel + i);
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter, CurrentLevel + i);
			}
			
			IPlayerInterface::Execute_AddToAttributesPoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			bTopOffHealth = true;
			bTopOffMana = true;
				
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
			
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UAuraAttributeSet::HandleReaction(const FEffectProperties& Props)
{
	static thread_local int32 ReactionCallDepth = 0;
	++ReactionCallDepth;

	ON_SCOPE_EXIT
	{
		--ReactionCallDepth;
	};

	UE_LOG(LogTemp, Warning, TEXT("HandleReaction entered. Depth: %d"), ReactionCallDepth);

	if (ReactionCallDepth >= 8)
	{
		UE_LOG(LogTemp, Error, TEXT("Synchronous reaction recursion confirmed. Breaking chain at depth %d."), ReactionCallDepth);

		return; // Diagnostic guard only
	}

	// Existing HandleReaction code...
	
	const FGameplayTag ReactionTag = UAuraAbilitySystemLibrary::GetTriggeredReaction(Props.EffectContextHandle);

	const FGameplayTag StatusToConsume = UAuraAbilitySystemLibrary::GetReactiveStatusToConsume(Props.EffectContextHandle);

	if (!ReactionTag.IsValid() || !StatusToConsume.IsValid() || !Props.TargetASC)
	{
		return;
	}

	FGameplayTagContainer TagsToRemove;
	TagsToRemove.AddTag(StatusToConsume);

	const int32 NumRemoved = Props.TargetASC->RemoveActiveEffectsWithGrantedTags(TagsToRemove);

	const int32 RemainingTagCount = Props.TargetASC->GetGameplayTagCount(StatusToConsume);

	UE_LOG(LogTemp, Warning,
		TEXT(
			"Reaction depth=%d Target=%s Status=%s "
			"NumRemoved=%d RemainingCount=%d Reaction=%s"),
		ReactionCallDepth, *GetNameSafe(Props.TargetAvatarActor), *StatusToConsume.ToString(), NumRemoved, RemainingTagCount,
		*ReactionTag.ToString());

	if (NumRemoved <= 0)
	{
		return;
	}

	// Dispatch reaction after Charged has been removed.
	FGameplayEventData Payload;
	Payload.EventTag = ReactionTag;
	Payload.Instigator = Props.SourceCharacter->GetInstigator();
	Payload.Target = Props.TargetAvatarActor;
	Payload.ContextHandle = Props.EffectContextHandle;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter->GetInstigator(), ReactionTag,Payload);
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		SetHealth(GetMaxHealth());
		bTopOffHealth = false;
	}

	if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		SetMana(GetMaxMana());
		bTopOffMana = false;
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetAvatarActor->Implements<UCombatInterface>())
	{
		const int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		const ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, TargetClass, TargetLevel);

		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, GameplayTags.Attributes_Meta_IncomingXP, Payload);
	}
}


void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter && Damage > 1.f)
	{
		if (Props.SourceCharacter != nullptr)
		{
			if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
			{
				PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
				return;
			}
		}
		if (Props.TargetCharacter != nullptr)
		{
			if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
			{
				PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			}
		}
	}
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Resilience, OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Vigor, OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ManaRegeneration, OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, FireResistance, OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, LightningResistance, OldLightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, ArcaneResistance, OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, PhysicalResistance, OldPhysicalResistance);
}

void UAuraAttributeSet::OnRep_SpeedBuff(const FGameplayAttributeData& OldSpeedBuff) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, SpeedBuff, OldSpeedBuff);
}
