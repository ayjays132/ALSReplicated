#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TraversalSmartRouter.generated.h"

UENUM(BlueprintType)
enum class ETraversalAction : uint8
{
    None,
    Mantle,
    LedgeGrab,
    Zipline,
    WallClimb
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTraversalActionChosen, ETraversalAction, Action);

/**
 * Decides which traversal action should be performed based on traces around the character.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UTraversalSmartRouter : public UActorComponent
{
    GENERATED_BODY()
public:
    UTraversalSmartRouter();

    /** Check the surroundings and return the suggested traversal action. */
    UFUNCTION(BlueprintCallable, Category="Traversal")
    ETraversalAction EvaluateTraversal();

    /** Broadcasts whenever EvaluateTraversal chooses an action. */
    UPROPERTY(BlueprintAssignable)
    FTraversalActionChosen OnTraversalActionChosen;

protected:
    virtual void BeginPlay() override;

    void PerformTrace(FHitResult& Hit) const;

    UPROPERTY(EditDefaultsOnly, Category="Traversal")
    float TraceDistance = 150.f;

    UPROPERTY(EditDefaultsOnly, Category="Traversal")
    float MaxMantleSlope = 40.f;
};

