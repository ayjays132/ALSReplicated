#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PerformAttack.generated.h"

/** Triggers a light attack using the combat component */
UCLASS()
class ALSREPLICATED_API UBTTask_PerformAttack : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_PerformAttack();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector TargetKey;
};
