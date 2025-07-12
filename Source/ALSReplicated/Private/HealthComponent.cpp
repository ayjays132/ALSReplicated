#include "HealthComponent.h"

UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
    bWasDead = Health <= 0.f;
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UHealthComponent, Health);
    DOREPLIFETIME(UHealthComponent, MaxHealth);
}

void UHealthComponent::AddHealth(float Amount)
{
    const bool bBefore = Health <= 0.f;
    Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
    OnRep_Health();
    OnHealthChanged.Broadcast(Health);
    if (bBefore && Health > 0.f)
    {
        OnRevived.Broadcast();
    }
    else if (!bBefore && Health <= 0.f)
    {
        OnDeath.Broadcast();
    }
}

void UHealthComponent::SetMaxHealth(float NewMax)
{
    MaxHealth = NewMax;
    Health = FMath::Clamp(Health, 0.f, MaxHealth);
    OnRep_MaxHealth();
    OnHealthChanged.Broadcast(Health);
}

void UHealthComponent::OnRep_Health()
{
    bool bDead = Health <= 0.f;
    if (bDead && !bWasDead)
    {
        OnDeath.Broadcast();
    }
    else if (!bDead && bWasDead)
    {
        OnRevived.Broadcast();
    }
    OnHealthChanged.Broadcast(Health);
    bWasDead = bDead;
}

void UHealthComponent::OnRep_MaxHealth()
{
    Health = FMath::Clamp(Health, 0.f, MaxHealth);
}

