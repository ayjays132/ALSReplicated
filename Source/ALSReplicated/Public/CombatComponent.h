#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "Net/UnrealNetwork.h"
#include "ALSCharacterMovementComponent.h"

class UStaminaComponent;
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

    /** Performs a dash using the DashMontage */
    UFUNCTION(BlueprintCallable, Category="Combat")
    void PerformDash();

    /** Add stamina back. Can be used by other systems */
    UFUNCTION(BlueprintCallable, Category="Combat")
    void AddStamina(float Amount);

    /** Change the maximum stamina value */
    UFUNCTION(BlueprintCallable, Category="Combat")
    void SetMaxStamina(float NewMax);

    /** Event fired for each hit detected during SpawnHitbox */
    UFUNCTION(BlueprintImplementableEvent, Category="Combat")
    void OnHitDetected(const FHitResult& Hit);

    /** Notifies when attacking begins or ends. bAttacking is the new state. */
    UFUNCTION(BlueprintImplementableEvent, Category="Combat")
    void OnAttackStateChanged(bool bAttacking);

protected:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerStartAttack(bool bHeavy);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerEquipWeapon(AActor* Weapon, FName SocketName);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerUnequipWeapon();

    UFUNCTION(Server, Reliable)
    void ServerPerformDash();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastPerformDash();

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


    UPROPERTY()
    UStaminaComponent* StaminaComponent = nullptr;

    FTimerHandle ComboTimerHandle;
    FTimerHandle AttackTimerHandle;
    FTimerHandle CooldownTimerHandle;
    FTimerHandle DashTimerHandle;

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

    /** Montage used when performing a dash */
    UPROPERTY(EditDefaultsOnly, Category="Combat")
    UAnimMontage* DashMontage = nullptr;

    /** If true the dash montage will use root motion */
    UPROPERTY(Replicated, EditDefaultsOnly, Category="Combat")
    bool bUseDashRootMotion = false;

    UPROPERTY()
    AActor* EquippedWeapon = nullptr;

    FName EquippedSocket;
};

