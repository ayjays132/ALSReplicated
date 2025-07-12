#include "WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UWeaponComponent, Damage);
    DOREPLIFETIME(UWeaponComponent, Range);
    DOREPLIFETIME(UWeaponComponent, StaminaCost);
    DOREPLIFETIME(UWeaponComponent, AttackSpeed);
}

