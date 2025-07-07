#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ImpactEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHardImpactEvent, AActor*, Instigator, FVector, Location);

/** Subsystem that broadcasts when hard impacts occur in the world. */
UCLASS()
class ALSREPLICATED_API UImpactEventSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()
public:
    /** Event fired whenever a hard impact occurs. */
    UPROPERTY(BlueprintAssignable)
    FHardImpactEvent OnHardImpact;

    /** Broadcast an impact event. */
    void BroadcastImpact(AActor* Instigator, const FVector& Location)
    {
        OnHardImpact.Broadcast(Instigator, Location);
    }
};

