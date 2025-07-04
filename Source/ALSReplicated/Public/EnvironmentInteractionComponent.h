#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "EnvironmentInteractionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UEnvironmentInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEnvironmentInteractionComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void PushObject();

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void PullObject();

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void OpenDoor();

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void UseLever();

    UFUNCTION(BlueprintCallable, Category="Traversal")
    void GrabLedge();

    UFUNCTION(BlueprintCallable, Category="Traversal")
    void UseZipline();

protected:
    UFUNCTION(Server, Reliable)
    void ServerInteract(AActor* Target, const FString& Action);

    UFUNCTION()
    void OnRep_Interaction();

    void PerformTrace(FHitResult& Hit);
    void HandleInteraction(AActor* Target, const FString& Action);

    UPROPERTY(ReplicatedUsing=OnRep_Interaction)
    AActor* InteractedActor = nullptr;

    UPROPERTY(ReplicatedUsing=OnRep_Interaction)
    FString LastAction;
};

