#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UWeaponComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UInventoryComponent();

    UFUNCTION(BlueprintCallable, Category="Inventory")
    void AddWeapon(UWeaponComponent* Weapon);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    void EquipSlot(int32 Index, FName SocketName);

    UFUNCTION(BlueprintCallable, Category="Inventory")
    UWeaponComponent* GetCurrentWeapon() const { return CurrentWeapon; }

protected:
    UPROPERTY()
    TArray<UWeaponComponent*> WeaponSlots;

    UPROPERTY()
    UWeaponComponent* CurrentWeapon = nullptr;
};

