#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraShakeBase.h"
#include "Materials/MaterialInterface.h"
#include "HitReactionComponent.generated.h"

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
    Front,
    Back,
    Left,
    Right
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UHitReactionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHitReactionComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Hit")
    void ReceiveHit(FVector HitLocation, FVector HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout);

    UFUNCTION(BlueprintCallable, Category="Stamina")
    void AddStamina(float Amount);

    UFUNCTION(BlueprintCallable, Category="Stamina")
    void SetMaxStamina(float NewMax);

protected:
    UFUNCTION(Server, Reliable)
    void ServerReceiveHit(FVector_NetQuantize HitLocation, FVector_NetQuantizeNormal HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout);

    UFUNCTION(NetMulticast, Reliable)
    void MulticastPlayHit(FVector_NetQuantize HitLocation, FVector_NetQuantizeNormal HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout);

    void PlayHit(FVector HitLocation, FVector HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout);

    void StartSlowMotion();
    void RestoreTimeDilation();

    UFUNCTION()
    void OnRep_KnockedOut();

    void StartRagdoll();
    void StopRagdoll();

    UPROPERTY(ReplicatedUsing=OnRep_KnockedOut)
    bool bIsKnockedOut = false;

    UPROPERTY(Replicated)
    float Stamina = 100.f;

    UPROPERTY(EditDefaultsOnly, Category="Stamina")
    float MaxStamina = 100.f;

    UPROPERTY(EditDefaultsOnly, Category="Stamina")
    float BlockStaminaCost = 20.f;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* FrontHitMontage;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* BackHitMontage;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* LeftHitMontage;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* RightHitMontage;

    UPROPERTY(EditDefaultsOnly, Category="Animation")
    UAnimMontage* StaggerMontage;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UNiagaraSystem* FrontHitFX;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UNiagaraSystem* BackHitFX;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UNiagaraSystem* LeftHitFX;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UNiagaraSystem* RightHitFX;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    TSubclassOf<UCameraShakeBase> HitCameraShake;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    TSubclassOf<UCameraShakeBase> CriticalCameraShake;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    float CriticalSlowMoScale = 0.2f;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    float CriticalSlowMoTime = 0.5f;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    UMaterialInterface* ImpactDecal;

    UPROPERTY(EditDefaultsOnly, Category="FX")
    FVector DecalSize = FVector(10.f, 10.f, 10.f);

    UPROPERTY(EditDefaultsOnly, Category="FX")
    float DecalLifeSpan = 5.f;

    UPROPERTY(ReplicatedUsing=OnRep_Ragdoll)
    bool bIsRagdoll = false;

    UFUNCTION()
    void OnRep_Ragdoll();

    UFUNCTION(BlueprintCallable, Category="Hit")
    void SetRagdollActive(bool bActive);

    UFUNCTION(Server, Reliable)
    void ServerSetRagdollActive(bool bActive);

    FTimerHandle SlowMoTimerHandle;
};

