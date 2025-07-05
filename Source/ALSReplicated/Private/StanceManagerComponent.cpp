#include "StanceManagerComponent.h"
#include "ALSCharacterMovementComponent.h"
#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UStanceManagerComponent::UStanceManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UStanceManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        CachedMovement = Cast<UALSCharacterMovementComponent>(OwnerChar->GetCharacterMovement());
        CachedCombat = OwnerChar->FindComponentByClass<UCombatComponent>();
    }

    ApplyStance();
}

void UStanceManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UStanceManagerComponent, CurrentStance);
}

void UStanceManagerComponent::SetStance(EStanceMode NewStance)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetStance(NewStance);
        return;
    }

    CurrentStance = NewStance;
    ApplyStance();
}

void UStanceManagerComponent::ServerSetStance_Implementation(EStanceMode NewStance)
{
    SetStance(NewStance);
}

void UStanceManagerComponent::OnRep_Stance()
{
    ApplyStance();
}

void UStanceManagerComponent::ApplyStance()
{
    float Speed = NormalSpeed;
    float MaxStam = NormalMaxStamina;

    switch (CurrentStance)
    {
    case EStanceMode::Stealth:
        Speed = StealthSpeed;
        MaxStam = StealthMaxStamina;
        break;
    case EStanceMode::Aggressive:
        Speed = AggressiveSpeed;
        MaxStam = AggressiveMaxStamina;
        break;
    default:
        Speed = NormalSpeed;
        MaxStam = NormalMaxStamina;
        break;
    }

    if (CachedMovement)
    {
        CachedMovement->SetMaxWalkSpeed(Speed);
    }

    if (CachedCombat)
    {
        CachedCombat->SetMaxStamina(MaxStam);
    }
}


