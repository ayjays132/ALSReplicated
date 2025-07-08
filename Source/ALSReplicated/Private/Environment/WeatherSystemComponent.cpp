#include "Environment/WeatherSystemComponent.h"

UWeatherSystemComponent::UWeatherSystemComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UWeatherSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UWeatherSystemComponent, CurrentWeather);
}

void UWeatherSystemComponent::SetWeather(EWeatherType NewWeather)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetWeather(NewWeather);
        return;
    }
    if (CurrentWeather != NewWeather)
    {
        CurrentWeather = NewWeather;
        OnRep_Weather();
    }
}

bool UWeatherSystemComponent::ServerSetWeather_Validate(EWeatherType NewWeather)
{
    return true;
}

void UWeatherSystemComponent::ServerSetWeather_Implementation(EWeatherType NewWeather)
{
    SetWeather(NewWeather);
}

void UWeatherSystemComponent::OnRep_Weather()
{
    OnWeatherChanged.Broadcast(CurrentWeather);
}
