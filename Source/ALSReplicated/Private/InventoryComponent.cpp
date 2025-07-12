#include "InventoryComponent.h"
#include "WeaponComponent.h"
#include "CombatComponent.h"
#include "GameFramework/Actor.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::AddWeapon(UWeaponComponent* Weapon)
{
    if (Weapon)
    {
        WeaponSlots.Add(Weapon);
    }
}

void UInventoryComponent::EquipSlot(int32 Index, FName SocketName)
{
    if (!WeaponSlots.IsValidIndex(Index))
    {
        return;
    }

    CurrentWeapon = WeaponSlots[Index];

    if (AActor* Owner = GetOwner())
    {
        if (UCombatComponent* Combat = Owner->FindComponentByClass<UCombatComponent>())
        {
            Combat->EquipWeapon(CurrentWeapon, SocketName);
        }
    }
}

