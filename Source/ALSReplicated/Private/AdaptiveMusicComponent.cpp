#include "AdaptiveMusicComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UAdaptiveMusicComponent::UAdaptiveMusicComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UAdaptiveMusicComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        StateCoordinator = Owner->FindComponentByClass<UCharacterStateCoordinator>();
        if (StateCoordinator)
        {
            StateCoordinator->OnStateChanged.AddDynamic(this, &UAdaptiveMusicComponent::HandleStateChanged);
            HandleStateChanged(StateCoordinator->GetCharacterState());
        }
    }
}

void UAdaptiveMusicComponent::HandleStateChanged(ECharacterActivityState NewState)
{
    USoundBase* Cue = (NewState == ECharacterActivityState::Combat) ? CombatCue : ExplorationCue;
    PlayCue(Cue);
}

void UAdaptiveMusicComponent::PlayCue(USoundBase* Cue)
{
    if (CurrentComponent)
    {
        CurrentComponent->FadeOut(FadeDuration, 0.f);
        CurrentComponent->bAutoDestroy = true;
        CurrentComponent = nullptr;
    }

    if (Cue)
    {
        CurrentComponent = UGameplayStatics::SpawnSoundAttached(Cue, GetOwner()->GetRootComponent());
        if (CurrentComponent)
        {
            CurrentComponent->FadeIn(FadeDuration, 1.f);
        }
    }
}

