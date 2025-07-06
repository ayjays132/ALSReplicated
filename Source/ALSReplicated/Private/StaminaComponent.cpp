#include "StaminaComponent.h"

UStaminaComponent::UStaminaComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UStaminaComponent::BeginPlay()
{
    Super::BeginPlay();
    Stamina = MaxStamina;
    bWasExhausted = Stamina <= 0.f;
}

void UStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UStaminaComponent, Stamina);
    DOREPLIFETIME(UStaminaComponent, MaxStamina);
}

void UStaminaComponent::AddStamina(float Amount)
{
    const bool bBefore = Stamina <= 0.f;
    Stamina = FMath::Clamp(Stamina + Amount, 0.f, MaxStamina);
    OnRep_Stamina();
    if (bBefore && Stamina > 0.f)
    {
        OnStaminaRecovered.Broadcast();
    }
    else if (!bBefore && Stamina <= 0.f)
    {
        OnStaminaDepleted.Broadcast();
    }
}

void UStaminaComponent::SetMaxStamina(float NewMax)
{
    MaxStamina = NewMax;
    Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
    OnRep_MaxStamina();
}

void UStaminaComponent::OnRep_Stamina()
{
    bool bExhausted = Stamina <= 0.f;
    if (bExhausted && !bWasExhausted)
    {
        OnStaminaDepleted.Broadcast();
    }
    else if (!bExhausted && bWasExhausted)
    {
        OnStaminaRecovered.Broadcast();
    }
    bWasExhausted = bExhausted;
}

void UStaminaComponent::OnRep_MaxStamina()
{
    Stamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
}


