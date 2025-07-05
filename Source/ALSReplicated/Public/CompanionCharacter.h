#pragma once

#include "CoreMinimal.h"
#include "ALSBaseCharacter.h"
#include "CompanionCharacter.generated.h"

UCLASS()
class ALSREPLICATED_API ACompanionCharacter : public AALSBaseCharacter
{
    GENERATED_BODY()
public:
    ACompanionCharacter(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category="Companion")
    void SetFollowTarget(AActor* Target);

    UFUNCTION(BlueprintCallable, Category="Companion")
    void AssistAttack();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void FollowTarget();

    UPROPERTY()
    AActor* FollowActor = nullptr;

    FTimerHandle FollowTimerHandle;
};


