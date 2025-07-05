#pragma once

#include "CoreMinimal.h"
#include "AI/AAAController.h"
#include "ALSAIController.generated.h"

class UBehaviorTree;

/** AI controller that runs a behavior tree on possess */
UCLASS()
class ALSREPLICATED_API AALSAIController : public AALSBaseAIController
{
    GENERATED_BODY()
public:
    AALSAIController();
protected:
    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(EditDefaultsOnly, Category="AI")
    UBehaviorTree* BehaviorTreeAsset;
};
