#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	/* Debuff Properties */
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	float ReactiveStatusChance = 0.f;

	/* Knockback Properties */
	UPROPERTY(BlueprintReadWrite)
	float KnockBackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockBackImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockBackImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldHitReact = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bCanApplyReactionStatus = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bCanTriggerReaction = true;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool isCriticalHit() const { return bIsCriticalHit; }
	bool isBlockedHit() const { return bIsBlockedHit; }
	bool GetShouldHitReact() const { return bShouldHitReact; }
	bool GetCanApplyReactionStatus() const { return bCanApplyReactionStatus; }
	bool GetCanTriggerReaction() const { return bCanTriggerReaction; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	bool IsSuccessfulReactiveStatus() const { return bIsSuccessfulReactiveStatus; }
	bool IsSuccessfulKnockback() const { return bIsSuccessfulKnockback; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	FGameplayTag GetTriggeredReaction() const { return TriggeredReaction; }
	FGameplayTag GetReactiveStatusToConsume() const { return ReactiveStatusToConsume; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackImpulse() const { return KnockBackImpulse; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	
	void SetIsSuccessfulDebuff(bool bInIsSuccessfulDebuff ) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetIsSuccessfulReactiveStatus(bool bInIsSuccessfulReactiveStatus ) { bIsSuccessfulReactiveStatus = bInIsSuccessfulReactiveStatus; }
	void SetTriggeredReaction(FGameplayTag InTriggeredReaction) { TriggeredReaction = InTriggeredReaction; }
	void SetReactiveStatusToConsume(FGameplayTag InReactiveStatusToConsume) { ReactiveStatusToConsume = InReactiveStatusToConsume; }
	
	void SetSuccessfulReactiveStatus(FGameplayTag InReactiveStatus ) { ReactiveStatus = InReactiveStatus; }
	void SetIsSuccessfulKnockback(bool bInIsSuccessfulKnockback ) { bIsSuccessfulKnockback = bInIsSuccessfulKnockback; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFreq) { DebuffFrequency = InFreq; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetShouldHitReact(bool InShouldHitReact) { bShouldHitReact = InShouldHitReact; }
	void SetReactiveStatusChance(float InReactiveStatusChance) { ReactiveStatusChance = InReactiveStatusChance; }
	void SetCanApplyReactionStatus(bool InCanApplyReactionStatus) { bCanApplyReactionStatus = InCanApplyReactionStatus; }
	void SetCanTriggerReaction(bool InCanTriggerReaction) { bCanTriggerReaction = InCanTriggerReaction; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackImpulse(const FVector& InKnockbackImpulse) { KnockBackImpulse = InKnockbackImpulse; }
	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	
	
	/** Returns the actual struct represented by this context. */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FAuraGameplayEffectContext::StaticStruct();
	}

	/** Creates a complete copy of the custom context. */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext =
			new FAuraGameplayEffectContext();

		*NewContext = *this;

		if (const FHitResult* ExistingHitResult = GetHitResult())
		{
			NewContext->AddHitResult(*ExistingHitResult, true);
		}

		return NewContext;
	}
	
	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	
protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	
	UPROPERTY()
	bool bIsSuccessfulReactiveStatus = false;
	
	UPROPERTY()
	FGameplayTag ReactiveStatus;
	
	UPROPERTY()
	float ReactiveStatusChance = 0.f;
	
	UPROPERTY()
	FGameplayTag TriggeredReaction;

	UPROPERTY()
	FGameplayTag ReactiveStatusToConsume;
	
	UPROPERTY()
	bool bCanApplyReactionStatus = true;
	
	UPROPERTY()
	bool bCanTriggerReaction = true;

	UPROPERTY()
	bool bIsSuccessfulKnockback = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;

	bool bShouldHitReact = false;
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockBackImpulse = FVector::ZeroVector;

	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerialize = true,
		WithCopy = true
	};
};
