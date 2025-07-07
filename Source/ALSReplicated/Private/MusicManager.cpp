#include "MusicManager.h"
#include "CharacterStateCoordinator.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"

UMusicManager::UMusicManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMusicManager::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* Owner = GetOwner())
    {
        StateCoordinator = Owner->FindComponentByClass<UCharacterStateCoordinator>();
        if (StateCoordinator)
        {
            StateCoordinator->OnStateChanged.AddDynamic(this, &UMusicManager::HandleStateChanged);
            HandleStateChanged(StateCoordinator->GetCharacterState());
        }
    }
}

void UMusicManager::HandleStateChanged(ECharacterActivityState NewState)
{
    switch (NewState)
    {
    case ECharacterActivityState::Combat:
        StartMusic(CombatBase, CombatIntensity);
        break;
    default:
        StartMusic(ExplorationBase, ExplorationIntensity);
        break;
    }
}

void UMusicManager::StartMusic(USoundBase* Base, USoundBase* Intense)
{
    if (BaseComponent)
    {
        BaseComponent->FadeOut(FadeDuration, 0.f);
        BaseComponent->bAutoDestroy = true;
        BaseComponent = nullptr;
    }
    if (IntensityComponent)
    {
        IntensityComponent->FadeOut(FadeDuration, 0.f);
        IntensityComponent->bAutoDestroy = true;
        IntensityComponent = nullptr;
    }

    if (Base)
    {
        BaseComponent = UGameplayStatics::SpawnSoundAttached(Base, GetOwner()->GetRootComponent());
        if (BaseComponent)
        {
            BaseComponent->FadeIn(FadeDuration, 1.f);
        }
    }

    if (Intense)
    {
        IntensityComponent = UGameplayStatics::SpawnSoundAttached(Intense, GetOwner()->GetRootComponent());
        if (IntensityComponent)
        {
            IntensityComponent->FadeIn(FadeDuration, Intensity);
        }
    }
}

void UMusicManager::SetIntensity(float NewIntensity)
{
    Intensity = FMath::Clamp(NewIntensity, 0.f, 1.f);
    if (IntensityComponent)
    {
        IntensityComponent->SetVolumeMultiplier(Intensity);
    }
}

void UMusicManager::PlayStinger(USoundBase* Stinger)
{
    if (Stinger)
    {
        UGameplayStatics::SpawnSoundAttached(Stinger, GetOwner()->GetRootComponent());
    }
}

