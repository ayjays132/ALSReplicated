#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "CharacterStateCoordinator.generated.h"

UENUM(BlueprintType)
enum class ECharacterActivityState : uint8
{
    Exploration,
    Stealth,
    Combat
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterStateEvent);

/**
 * Coordinates overall character state and broadcasts events when key changes occur.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UCharacterStateCoordinator : public UActorComponent
{
    GENERATED_BODY()
public:
    UCharacterStateCoordinator();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="State")
    void SetCharacterState(ECharacterActivityState NewState);

    UFUNCTION(BlueprintCallable, Category="State")
    ECharacterActivityState GetCharacterState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category="State")
    void NotifyTraversalAction();

    UPROPERTY(BlueprintAssignable)
    FCharacterStateEvent OnCombatEngaged;

    UPROPERTY(BlueprintAssignable)
    FCharacterStateEvent OnStaminaCritical;

    UPROPERTY(BlueprintAssignable)
    FCharacterStateEvent OnTraversalAction;

protected:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerSetCharacterState(ECharacterActivityState NewState);

    UFUNCTION()
    void OnRep_State();

    UFUNCTION()
    void HandleStaminaDepleted();

    UPROPERTY(ReplicatedUsing=OnRep_State)
    ECharacterActivityState CurrentState = ECharacterActivityState::Exploration;

    UPROPERTY()
    class UStaminaComponent* CachedStamina = nullptr;
};

