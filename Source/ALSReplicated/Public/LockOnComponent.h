#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "LockOnComponent.generated.h"

class UUserWidget;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API ULockOnComponent : public UActorComponent
{
        GENERATED_BODY()

public:
        ULockOnComponent();

        virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

        UFUNCTION(BlueprintCallable, Category="LockOn")
        void ToggleLockOn();

protected:
        UFUNCTION(Server, Reliable)
        void ServerToggleLockOn();

        void DoToggleLockOn();
        void PerformLineTrace();

        UPROPERTY(Replicated)
        bool bIsLockedOn = false;

        UPROPERTY(Replicated)
        AActor* LockedActor = nullptr;

public:
        UPROPERTY(EditDefaultsOnly, Category="LockOn")
        TSubclassOf<UUserWidget> TargetWidgetClass;
};

