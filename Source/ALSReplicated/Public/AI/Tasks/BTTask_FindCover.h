#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindCover.generated.h"

/** Simple task that finds a random reachable location around the controlled pawn. */
UCLASS()
class ALSREPLICATED_API UBTTask_FindCover : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_FindCover();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    /** Radius to search for cover */
    UPROPERTY(EditAnywhere, Category="Search")
    float SearchRadius;

    /** Blackboard key to store the location */
    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector LocationKey;
};

