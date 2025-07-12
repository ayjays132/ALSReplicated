#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "CinematicTriggerComponent.generated.h"

class ULevelSequence;
class UMovieSceneSequencePlayer;
class APlayerController;
class ALevelSequenceActor;

/**
 * Component that plays a level sequence when the player overlaps the box.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCinematicFinished);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ALSREPLICATED_API UCinematicTriggerComponent : public UBoxComponent
{
    GENERATED_BODY()
public:
    UCinematicTriggerComponent();

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                        const FHitResult& SweepResult);

    UFUNCTION()
    void OnSequenceFinished();

    void PlayCinematic(APlayerController* PC);

    UPROPERTY(EditAnywhere, Replicated, Category="Cinematic")
    ULevelSequence* Sequence = nullptr;

    UPROPERTY()
    UMovieSceneSequencePlayer* SequencePlayer = nullptr;

    UPROPERTY()
    APlayerController* CachedController = nullptr;

    UPROPERTY()
    ALevelSequenceActor* SequenceActor = nullptr;

    UPROPERTY(ReplicatedUsing=OnRep_Playing)
    bool bPlaying = false;

    UFUNCTION()
    void OnRep_Playing();

public:
    /** Called when the cinematic finishes playing */
    UPROPERTY(BlueprintAssignable, Category="Cinematic")
    FCinematicFinished OnCinematicFinished;
};

