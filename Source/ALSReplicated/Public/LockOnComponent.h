#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "UObject/NetGUID.h"

#include "LockOnComponent.generated.h"

class UUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API ULockOnComponent : public UActorComponent
{
        GENERATED_BODY()

public:
        ULockOnComponent();

        virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
        virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

        UFUNCTION(BlueprintCallable, Category="LockOn")
        void ToggleLockOn();

        UFUNCTION(BlueprintCallable, Category="LockOn")
        APawn* GetCurrentTarget() const { return CurrentTarget; }

        UFUNCTION(BlueprintImplementableEvent, Category="LockOn")
        void ShowReticle(APawn* Target);

        UFUNCTION(BlueprintImplementableEvent, Category="LockOn")
        void HideReticle();

protected:
        UFUNCTION(Server, Reliable)
        void ServerToggleLockOn();

       void DoToggleLockOn();
       APawn* FindNearestTarget() const;
       bool HasLineOfSight(APawn* Target) const;

       UFUNCTION()
       void OnRep_LockedTarget();

       UFUNCTION()
       void OnRep_TickActive();

       UPROPERTY(ReplicatedUsing=OnRep_LockedTarget)
       FNetworkGUID LockedTargetId;

       UPROPERTY(Transient)
       APawn* CurrentTarget = nullptr;

       UPROPERTY(ReplicatedUsing=OnRep_TickActive)
       bool bTickActive = false;

       UPROPERTY(EditDefaultsOnly, Category="LockOn")
       float TraceRange = 2000.0f;

public:
       UPROPERTY(EditDefaultsOnly, Category="LockOn")
       TSubclassOf<UUserWidget> TargetWidgetClass;
};

