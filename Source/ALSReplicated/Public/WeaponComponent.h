#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "WeaponComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UWeaponComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UWeaponComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Weapon")
    float Damage = 25.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Weapon")
    float Range = 150.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Weapon")
    float StaminaCost = 10.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Weapon")
    float AttackSpeed = 1.f;
};

