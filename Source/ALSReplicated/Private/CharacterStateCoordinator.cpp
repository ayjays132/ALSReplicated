#include "CharacterStateCoordinator.h"
#include "StaminaComponent.h"
#include "GameFramework/Actor.h"

UCharacterStateCoordinator::UCharacterStateCoordinator()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UCharacterStateCoordinator::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        CachedStamina = Owner->FindComponentByClass<UStaminaComponent>();
        if (CachedStamina)
        {
            CachedStamina->OnStaminaDepleted.AddDynamic(this, &UCharacterStateCoordinator::HandleStaminaDepleted);
        }
    }
}

void UCharacterStateCoordinator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCharacterStateCoordinator, CurrentState);
}

void UCharacterStateCoordinator::SetCharacterState(ECharacterActivityState NewState)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetCharacterState(NewState);
        return;
    }
    if (CurrentState != NewState)
    {
        CurrentState = NewState;
        OnStateChanged.Broadcast(CurrentState);
        OnRep_State();
    }
}

void UCharacterStateCoordinator::ServerSetCharacterState_Implementation(ECharacterActivityState NewState)
{
    SetCharacterState(NewState);
}

void UCharacterStateCoordinator::OnRep_State()
{
    if (CurrentState == ECharacterActivityState::Combat)
    {
        OnCombatEngaged.Broadcast();
    }

    if (GetOwnerRole() != ROLE_Authority)
    {
        OnStateChanged.Broadcast(CurrentState);
    }
}

void UCharacterStateCoordinator::NotifyTraversalAction()
{
    OnTraversalAction.Broadcast();
}

void UCharacterStateCoordinator::HandleStaminaDepleted()
{
    OnStaminaCritical.Broadcast();
}

