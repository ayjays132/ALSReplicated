#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "Net/UnrealNetwork.h"
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
    UAnimMontage* LightAttackMontage;

    UPROPERTY(EditDefaultsOnly, Category="Combat")
    UAnimMontage* HeavyAttackMontage;

    UPROPERTY()
    AActor* EquippedWeapon = nullptr;

    FName EquippedSocket;
};

