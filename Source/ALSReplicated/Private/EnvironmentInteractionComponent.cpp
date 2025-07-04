#include "EnvironmentInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UEnvironmentInteractionComponent::UEnvironmentInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UEnvironmentInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UEnvironmentInteractionComponent, InteractedActor);
    DOREPLIFETIME(UEnvironmentInteractionComponent, LastAction);
}

void UEnvironmentInteractionComponent::PerformTrace(FHitResult& Hit)
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return;
    }

    FVector Start;
    FRotator Rot;
    OwnerActor->GetActorEyesViewPoint(Start, Rot);
    FVector End = Start + Rot.Vector() * 300.f;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerActor);

    GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
}

void UEnvironmentInteractionComponent::PushObject()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        ServerInteract(Hit.GetActor(), TEXT("Push"));
    }
}

void UEnvironmentInteractionComponent::PullObject()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        ServerInteract(Hit.GetActor(), TEXT("Pull"));
    }
}

void UEnvironmentInteractionComponent::OpenDoor()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        ServerInteract(Hit.GetActor(), TEXT("OpenDoor"));
    }
}

void UEnvironmentInteractionComponent::UseLever()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        ServerInteract(Hit.GetActor(), TEXT("UseLever"));
    }
}

void UEnvironmentInteractionComponent::GrabLedge()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        ServerInteract(Hit.GetActor(), TEXT("GrabLedge"));
    }
}

void UEnvironmentInteractionComponent::UseZipline()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        ServerInteract(Hit.GetActor(), TEXT("UseZipline"));
    }
}

void UEnvironmentInteractionComponent::ServerInteract_Implementation(AActor* Target, const FString& Action)
{
    InteractedActor = Target;
    LastAction = Action;

    HandleInteraction(Target, Action);
}

void UEnvironmentInteractionComponent::HandleInteraction(AActor* Target, const FString& Action)
{
    if (!Target)
    {
        return;
    }

    if (Action == TEXT("Push"))
    {
        Target->AddActorWorldOffset(Target->GetActorForwardVector() * 50.f);
    }
    else if (Action == TEXT("Pull"))
    {
        Target->AddActorWorldOffset(-Target->GetActorForwardVector() * 50.f);
    }
    else if (Action == TEXT("OpenDoor"))
    {
        Target->AddActorWorldRotation(FRotator(0.f, 90.f, 0.f));
    }
    else if (Action == TEXT("UseLever"))
    {
        Target->AddActorWorldRotation(FRotator(-45.f, 0.f, 0.f));
    }
    else if (Action == TEXT("GrabLedge"))
    {
        // Placeholder: actual ledge grab logic should move the character
    }
    else if (Action == TEXT("UseZipline"))
    {
        // Placeholder: actual zipline logic should handle movement
    }
}

void UEnvironmentInteractionComponent::OnRep_Interaction()
{
    HandleInteraction(InteractedActor, LastAction);
}

