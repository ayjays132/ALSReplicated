#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, float, NewHealth);

/** Replicated component storing current and max health */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UHealthComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Adds the specified amount to health (negative to damage). */
    UFUNCTION(BlueprintCallable, Category="Health")
    void AddHealth(float Amount);

    /** Change the maximum health value */
    UFUNCTION(BlueprintCallable, Category="Health")
    void SetMaxHealth(float NewMax);

    /** Current health value */
    UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category="Health")
    float Health = 100.f;

    /** Maximum health value */
    UPROPERTY(ReplicatedUsing=OnRep_MaxHealth, EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    float MaxHealth = 100.f;

    /** Fired when health reaches zero */
    UPROPERTY(BlueprintAssignable)
    FHealthEvent OnDeath;

    /** Fired when health is recovered from zero */
    UPROPERTY(BlueprintAssignable)
    FHealthEvent OnRevived;

    /** Fired whenever the health value changes */
    UPROPERTY(BlueprintAssignable)
    FHealthChanged OnHealthChanged;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnRep_Health();

    UFUNCTION()
    void OnRep_MaxHealth();

    bool bWasDead = false;
};

