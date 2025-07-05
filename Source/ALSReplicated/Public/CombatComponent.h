#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "Net/UnrealNetwork.h"
#include "ALSCharacterMovementComponent.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCombatComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Combat")
    void LightAttack();

    UFUNCTION(BlueprintCallable, Category="Combat")
    void HeavyAttack();

    UFUNCTION(BlueprintCallable, Category="Combat")
    void EquipWeapon(AActor* Weapon, FName SocketName);

    UFUNCTION(BlueprintCallable, Category="Combat")
    void UnequipWeapon();

    UFUNCTION(BlueprintCallable, Category="Combat")
    void SpawnHitbox();

    /** Add stamina back. Can be used by other systems */
    UFUNCTION(BlueprintCallable, Category="Combat")
    void AddStamina(float Amount);

    /** Change the maximum stamina value */
    UFUNCTION(BlueprintCallable, Category="Combat")
    void SetMaxStamina(float NewMax);

    /** Event fired for each hit detected during SpawnHitbox */
    UFUNCTION(BlueprintImplementableEvent, Category="Combat")
    void OnHitDetected(const FHitResult& Hit);

    /**
     * Called whenever the attack state changes.
     * Implement in Blueprint to play VFX, sounds or UI feedback
     * when an attack starts (bAttacking = true) or ends.
     */
    UFUNCTION(BlueprintImplementableEvent, Category="Combat")
    void OnAttackStateChanged(bool bAttacking);

protected:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerStartAttack(bool bHeavy);

    void DoAttack(bool bHeavy);
    void FinishAttack();
    void ResetCombo();

    UFUNCTION()
    void OnRep_AttackState();

    UPROPERTY(ReplicatedUsing=OnRep_AttackState)
    bool bIsAttacking = false;

    UPROPERTY(Replicated)
    int32 ComboIndex = 0;

    UPROPERTY(Replicated)
    float AttackCooldown = 0.f;

    UPROPERTY(Replicated)
    float Stamina = 100.f;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float MaxStamina = 100.f;

    FTimerHandle ComboTimerHandle;
    FTimerHandle AttackTimerHandle;
    FTimerHandle CooldownTimerHandle;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float LightAttackStaminaCost = 20.f;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float HeavyAttackStaminaCost = 40.f;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float ComboResetTime = 1.5f;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float AttackDuration = 0.5f;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    float AttackWalkSpeed = 200.f;

    float PreviousWalkSpeed = 0.f;

    UPROPERTY()
    class UALSCharacterMovementComponent* CachedMovement;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    UAnimMontage* LightAttackMontage;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    UAnimMontage* HeavyAttackMontage;

    UPROPERTY()
    AActor* EquippedWeapon = nullptr;

    FName EquippedSocket;
};

