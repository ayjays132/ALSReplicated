#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetPatrolPoint.generated.h"

/** Finds a random patrol point around the pawn */
UCLASS()
class ALSREPLICATED_API UBTTask_GetPatrolPoint : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_GetPatrolPoint();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category="Search")
    float SearchRadius;

    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector LocationKey;
};
