#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EmotionStateComponent.h"
#include "BTTask_SetEmotion.generated.h"

/** Sets the emotion state on the controller */
UCLASS()
class ALSREPLICATED_API UBTTask_SetEmotion : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_SetEmotion();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category="Emotion")
    EEmotionState NewEmotion;
};
