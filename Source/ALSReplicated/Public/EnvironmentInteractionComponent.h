#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "ALSCharacterMovementComponent.h"
#include "Components/SplineComponent.h"
#include "CableComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraShakeBase.h"
#include "Materials/MaterialInterface.h"
#include "TraversalSmartRouter.h"
#include "EnvironmentInteractionComponent.generated.h"

UENUM(BlueprintType)
enum class EInteractionAction : uint8
{
    None,
    Push,
    Pull,
    OpenDoor,
    UseLever,
    GrabLedge,
    UseZipline
};

USTRUCT()
struct FInteractionInfo
{
    GENERATED_BODY()

    UPROPERTY()
    AActor* Actor = nullptr;

    UPROPERTY()
    TEnumAsByte<EInteractionAction> Action = EInteractionAction::None;
};

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
    UFUNCTION(Server, Reliable, WithValidation)
    void ServerInteract(AActor* Target, EInteractionAction Action);

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerBeginInteraction(EInteractionAction Action, float Duration);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastInteract(AActor* Target, EInteractionAction Action);

    UFUNCTION()
    void OnRep_Interaction();

    void PerformTrace(FHitResult& Hit);
    void HandleInteraction(AActor* Target, EInteractionAction Action);

    void BeginInteraction(EInteractionAction Action, float Duration = 1.0f);
    void EndInteraction();

    UPROPERTY(ReplicatedUsing=OnRep_Interaction)
    FInteractionInfo InteractionInfo;

    UPROPERTY(EditDefaultsOnly, Category="Interaction")
    FGameplayTag LedgeTag;

    UPROPERTY(Replicated)
    bool bIsInteracting = false;

    UPROPERTY()
    UALSCharacterMovementComponent* CachedMovement = nullptr;

    UPROPERTY()
    UTraversalSmartRouter* TraversalRouter = nullptr;

    UPROPERTY()
    USplineComponent* ActiveZipline = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UNiagaraSystem* InteractionFX = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UMaterialInterface* InteractionDecal = nullptr;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    FVector InteractionDecalSize = FVector(10.f, 10.f, 10.f);

    UPROPERTY(EditDefaultsOnly, Category="FX")
    float InteractionDecalLifeSpan = 5.f;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    TSubclassOf<UCameraShakeBase> HeavyImpactShake;

    FTimerHandle ZiplineTimerHandle;
    float ZiplineProgress = 0.f;
    float ZiplineSpeed = 600.f;

    void UpdateZiplineMovement();
    void StopZipline();
};

