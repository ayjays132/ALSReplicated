#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "StaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStaminaEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaChanged, float, NewStamina);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UStaminaComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UStaminaComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    /** Current stamina value */
    UPROPERTY(ReplicatedUsing=OnRep_Stamina, BlueprintReadOnly, Category="Stamina")
    float Stamina = 100.f;

    /** Maximum stamina value */
    UPROPERTY(ReplicatedUsing=OnRep_MaxStamina, EditDefaultsOnly, BlueprintReadOnly, Category="Stamina")
    float MaxStamina = 100.f;

    /** Adds the specified amount to stamina (negative to consume). */
    UFUNCTION(BlueprintCallable, Category="Stamina")
    void AddStamina(float Amount);

    /** Change the maximum stamina value */
    UFUNCTION(BlueprintCallable, Category="Stamina")
    void SetMaxStamina(float NewMax);

    /** Fired when stamina reaches zero */
    UPROPERTY(BlueprintAssignable)
    FStaminaEvent OnStaminaDepleted;

    /** Fired when stamina is recovered from zero */
    UPROPERTY(BlueprintAssignable)
    FStaminaEvent OnStaminaRecovered;

    /** Fired whenever the stamina value changes */
    UPROPERTY(BlueprintAssignable)
    FStaminaChanged OnStaminaChanged;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnRep_Stamina();

    UFUNCTION()
    void OnRep_MaxStamina();

    bool bWasExhausted = false;
};

