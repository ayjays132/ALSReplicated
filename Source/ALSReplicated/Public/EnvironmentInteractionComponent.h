#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "ALSCharacterMovementComponent.h"
#include "Components/SplineComponent.h"
#include "CableComponent.h"
#include "EnvironmentInteractionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UEnvironmentInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEnvironmentInteractionComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    virtual void BeginPlay() override;

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

    UFUNCTION(BlueprintCallable, Category="Interaction")
    void UseAction();

protected:
    UFUNCTION(Server, Reliable)
    void ServerInteract(AActor* Target, const FString& Action);

    UFUNCTION(Server, Reliable)
    void ServerBeginInteraction(const FString& Action, float Duration);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastInteract(AActor* Target, const FString& Action);

    UFUNCTION()
    void OnRep_Interaction();

    void PerformTrace(FHitResult& Hit);
    void HandleInteraction(AActor* Target, const FString& Action);

    void BeginInteraction(const FString& Action, float Duration = 1.0f);
    void EndInteraction();

    UPROPERTY(ReplicatedUsing=OnRep_Interaction)
    AActor* InteractedActor = nullptr;

    UPROPERTY(ReplicatedUsing=OnRep_Interaction)
    FString LastAction;

    UPROPERTY(EditDefaultsOnly, Category="Interaction")
    FGameplayTag LedgeTag;

    UPROPERTY(Replicated)
    bool bIsInteracting = false;

    UPROPERTY()
    UALSCharacterMovementComponent* CachedMovement = nullptr;

    UPROPERTY()
    USplineComponent* ActiveZipline = nullptr;

    FTimerHandle ZiplineTimerHandle;
    float ZiplineProgress = 0.f;
    float ZiplineSpeed = 600.f;

    void UpdateZiplineMovement();
    void StopZipline();
};

