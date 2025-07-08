#include "AtmosphereManager.h"
#include "Net/UnrealNetwork.h"

UAtmosphereManager::UAtmosphereManager()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UAtmosphereManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UAtmosphereManager, TimeOfDay);
    DOREPLIFETIME(UAtmosphereManager, bRaining);
    DOREPLIFETIME(UAtmosphereManager, bFoggy);
}

void UAtmosphereManager::SetTimeOfDay(float NewTime)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetTimeOfDay(NewTime);
        return;
    }
    TimeOfDay = FMath::Fmod(NewTime, 24.f);
    OnRep_TimeOfDay();
}

bool UAtmosphereManager::ServerSetTimeOfDay_Validate(float NewTime) { return true; }
void UAtmosphereManager::ServerSetTimeOfDay_Implementation(float NewTime) { SetTimeOfDay(NewTime); }

void UAtmosphereManager::SetRaining(bool bNewRaining)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetRaining(bNewRaining);
        return;
    }
    bRaining = bNewRaining;
    OnRep_Rain();
}

bool UAtmosphereManager::ServerSetRaining_Validate(bool bNewRaining) { return true; }
void UAtmosphereManager::ServerSetRaining_Implementation(bool bNewRaining) { SetRaining(bNewRaining); }

void UAtmosphereManager::SetFoggy(bool bNewFoggy)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerSetFoggy(bNewFoggy);
        return;
    }
    bFoggy = bNewFoggy;
    OnRep_Fog();
}

bool UAtmosphereManager::ServerSetFoggy_Validate(bool bNewFoggy) { return true; }
void UAtmosphereManager::ServerSetFoggy_Implementation(bool bNewFoggy) { SetFoggy(bNewFoggy); }

void UAtmosphereManager::OnRep_TimeOfDay()
{
    UpdateLighting();
    OnTimeOfDayChanged.Broadcast(TimeOfDay);
}

void UAtmosphereManager::OnRep_Rain()
{
    OnRainStateChanged.Broadcast(bRaining);
}

void UAtmosphereManager::OnRep_Fog()
{
    OnFogStateChanged.Broadcast(bFoggy);
}

void UAtmosphereManager::UpdateLighting()
{
    if (DirectionalLight)
    {
        FRotator Rot = FRotator((TimeOfDay / 24.f) * 360.f - 90.f, 0.f, 0.f);
        DirectionalLight->SetActorRotation(Rot);
    }
    if (SkyMaterial)
    {
        SkyMaterial->SetScalarParameterValue(SkyTimeParameterName, TimeOfDay);
    }
}

