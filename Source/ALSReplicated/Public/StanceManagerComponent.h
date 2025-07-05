#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StanceManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EStanceMode : uint8
{
    Stealth,
    Normal,
    Aggressive
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UStanceManagerComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UStanceManagerComponent();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(BlueprintCallable, Category="Stance")
    void SetStance(EStanceMode NewStance);

    UFUNCTION(BlueprintCallable, Category="Stance")
    EStanceMode GetStance() const { return CurrentStance; }

protected:
    virtual void BeginPlay() override;

    UFUNCTION(Server, Reliable)
    void ServerSetStance(EStanceMode NewStance);

    UFUNCTION()
    void OnRep_Stance();

    void ApplyStance();

    UPROPERTY(ReplicatedUsing=OnRep_Stance, EditAnywhere, BlueprintReadOnly, Category="Stance")
    EStanceMode CurrentStance = EStanceMode::Normal;

    // Movement speeds for each stance
    UPROPERTY(EditDefaultsOnly, Category="Stance|Movement")
    float StealthSpeed = 200.f;

    UPROPERTY(EditDefaultsOnly, Category="Stance|Movement")
    float NormalSpeed = 450.f;

    UPROPERTY(EditDefaultsOnly, Category="Stance|Movement")
    float AggressiveSpeed = 600.f;

    // Maximum stamina for each stance
    UPROPERTY(EditDefaultsOnly, Category="Stance|Combat")
    float StealthMaxStamina = 150.f;

    UPROPERTY(EditDefaultsOnly, Category="Stance|Combat")
    float NormalMaxStamina = 100.f;

    UPROPERTY(EditDefaultsOnly, Category="Stance|Combat")
    float AggressiveMaxStamina = 80.f;

    UPROPERTY()
    class UALSCharacterMovementComponent* CachedMovement = nullptr;

    UPROPERTY()
    class UCombatComponent* CachedCombat = nullptr;
};

