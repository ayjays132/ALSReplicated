#include "Environment/DayNightCycleComponent.h"

UDayNightCycleComponent::UDayNightCycleComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UDayNightCycleComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UDayNightCycleComponent, TimeOfDay);
}

void UDayNightCycleComponent::SetTimeOfDay(float NewTime)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetTimeOfDay(NewTime);
        return;
    }
    TimeOfDay = FMath::Fmod(NewTime, 24.f);
    OnRep_TimeOfDay();
}

bool UDayNightCycleComponent::ServerSetTimeOfDay_Validate(float NewTime)
{
    return true;
}

void UDayNightCycleComponent::ServerSetTimeOfDay_Implementation(float NewTime)
{
    SetTimeOfDay(NewTime);
}

void UDayNightCycleComponent::OnRep_TimeOfDay()
{
    OnTimeOfDayChanged.Broadcast(TimeOfDay);
}
