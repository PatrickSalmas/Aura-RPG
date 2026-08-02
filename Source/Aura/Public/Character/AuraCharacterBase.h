// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Materials/MaterialInstance.h"
#include "AuraCharacterBase.generated.h"

class AAuraAIController;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UAuraAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAuraCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	virtual void OnLanded(const FVector& HitLocation);

	/** Combat Interface */
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual FOnDeath* GetOnDeathDelegate() override;
	virtual FOnDeathSignature& GetOnDeathSignatureDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(bool isBeingShocked) override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;
	/** end Combat Interface */

	FOnASCRegistered OnASCRegistered;
	FOnDeath OnDeath;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Charged, BlueprintReadOnly)
	bool bIsCharged = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsImmobilized = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsSlowed = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsUnstable = false;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;
	
	UFUNCTION()
	virtual void OnRep_Stunned();
	
	UFUNCTION()
	virtual void OnRep_Charged();

	UFUNCTION()
	virtual void OnRep_Burned();

	UFUNCTION(BlueprintCallable)
	void AddToAttachActors(AActor* AttachActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CancelActions();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetIsBurningEvent(AActor* SourceActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetIsNotBurningEvent();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> WeaponR;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName TailSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftTentacle1SocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftTentacle2SocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightTentacle1SocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightTentacle2SocketName;

	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	bool bDead = false;

	virtual void StunTaggedChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	virtual void ChargeTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	virtual void BurningTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	virtual void ArcaneFireTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	virtual void UnstableTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	virtual void ImmobilizedTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	virtual void SlowedTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float CurrentWalkSpeed = BaseWalkSpeed;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultResistanceAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DestroyAttachedComponents();

	/* Dissolve Effects */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;

	/* Minions */

	int32 MinionCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleDefaultsOnly, Category="Niagara")
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Niagara")
	TObjectPtr<UDebuffNiagaraComponent> ArcaneFireDebuffComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Niagara")
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Niagara")
	TObjectPtr<UDebuffNiagaraComponent> ChargedDebuffComponent;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Niagara")
	TObjectPtr<UDebuffNiagaraComponent> UnstableDebuffComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Niagara")
	TObjectPtr<UDebuffNiagaraComponent> ImmobilizeDebuffComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Niagara")
	TObjectPtr<UDebuffNiagaraComponent> SlowDebuffComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Niagara")
	FLinearColor ChargedColor =FLinearColor(0.05f, 0.35f, 1.f, 1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Niagara")
	FLinearColor UnstableColor =FLinearColor(0.3f, 0.02f, 0.24f, 1.f);

	// UPROPERTY(BlueprintReadWrite)
	// TObjectPtr<UNiagaraComponent> ArcanePullComponent;

	UPROPERTY()
	TArray<AActor*> AttachedActors;
	
private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;
};
