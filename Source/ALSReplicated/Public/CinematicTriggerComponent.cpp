#include "CinematicTriggerComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"

UCinematicTriggerComponent::UCinematicTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SetGenerateOverlapEvents(true);
    SetCollisionResponseToAllChannels(ECR_Ignore);
    SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UCinematicTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
    OnComponentBeginOverlap.AddDynamic(this, &UCinematicTriggerComponent::OnOverlapBegin);
}

void UCinematicTriggerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UCinematicTriggerComponent, Sequence);
    DOREPLIFETIME(UCinematicTriggerComponent, bPlaying);
}

void UCinematicTriggerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult)
{
    if (bPlaying || !Sequence || GetOwnerRole() != ROLE_Authority)
    {
        return;
    }

    APawn* Pawn = Cast<APawn>(OtherActor);
    if (!Pawn)
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
    if (!PC)
    {
        return;
    }

    PlayCinematic(PC);
}

void UCinematicTriggerComponent::PlayCinematic(APlayerController* PC)
{
    if (!PC || !Sequence)
    {
        return;
    }

    bPlaying = true;
    CachedController = PC;
    PC->DisableInput(PC);

    FMovieSceneSequencePlaybackSettings Settings;
    SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, Settings, SequenceActor);
    if (SequencePlayer)
    {
        SequencePlayer->OnFinished.AddDynamic(this, &UCinematicTriggerComponent::OnSequenceFinished);
        SequencePlayer->Play();
    }
    else
    {
        OnSequenceFinished();
    }
}

void UCinematicTriggerComponent::OnRep_Playing()
{
    if (bPlaying && !SequencePlayer && Sequence)
    {
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
        {
            PlayCinematic(PC);
        }
    }
}

void UCinematicTriggerComponent::OnSequenceFinished()
{
    if (CachedController)
    {
        CachedController->EnableInput(CachedController);
        CachedController = nullptr;
    }

    bPlaying = false;
    OnCinematicFinished.Broadcast();
}

