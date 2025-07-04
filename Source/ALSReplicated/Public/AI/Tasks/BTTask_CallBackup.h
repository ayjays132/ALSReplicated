#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CallBackup.generated.h"

/** Task that triggers an event for backup. */
UCLASS()
class ALSREPLICATED_API UBTTask_CallBackup : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_CallBackup();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

