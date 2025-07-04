#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_CirclePlayer.generated.h"

/** Moves the AI pawn to a point around the current target */
UCLASS()
class ALSREPLICATED_API UBTTask_CirclePlayer : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_CirclePlayer();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category="Circle")
    float Radius;

    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector TargetKey;

    UPROPERTY(EditAnywhere, Category="Blackboard")
    FBlackboardKeySelector DestinationKey;
};

