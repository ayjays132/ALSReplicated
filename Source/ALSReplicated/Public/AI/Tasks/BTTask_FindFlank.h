#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindFlank.generated.h"

/** Finds a flanking location beside the target */
UCLASS()
class ALSREPLICATED_API UBTTask_FindFlank : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_FindFlank();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector DestinationKey;

    UPROPERTY(EditAnywhere, Category="Flank")
    float Distance;
};
