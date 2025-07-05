#include "EnvironmentInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameplayTagAssetInterface.h"
#include "Components/SplineComponent.h"
#include "CableComponent.h"
#include "Components/PrimitiveComponent.h"

UEnvironmentInteractionComponent::UEnvironmentInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
    LedgeTag = FGameplayTag::RequestGameplayTag(TEXT("Surface.Ledge"));
}

void UEnvironmentInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    if (AActor* OwnerActor = GetOwner())
    {
        if (APawn* Pawn = Cast<APawn>(OwnerActor))
        {
            CachedMovement = Cast<UALSCharacterMovementComponent>(Pawn->GetMovementComponent());
        }
    }
}

void UEnvironmentInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UEnvironmentInteractionComponent, InteractedActor);
    DOREPLIFETIME(UEnvironmentInteractionComponent, LastAction);
    DOREPLIFETIME(UEnvironmentInteractionComponent, bIsInteracting);
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

void UEnvironmentInteractionComponent::BeginInteraction(const FString& Action, float Duration)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerBeginInteraction(Action, Duration);
    }

    bIsInteracting = true;
    LastAction = Action;

    if (CachedMovement)
    {
        CachedMovement->DisableMovement();
    }

    if (GetWorld())
    {
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UEnvironmentInteractionComponent::EndInteraction, Duration, false);
    }
}

void UEnvironmentInteractionComponent::EndInteraction()
{
    bIsInteracting = false;

    if (CachedMovement)
    {
        CachedMovement->SetMovementMode(MOVE_Walking);
    }
}

void UEnvironmentInteractionComponent::ServerBeginInteraction_Implementation(const FString& Action, float Duration)
{
    BeginInteraction(Action, Duration);
}

bool UEnvironmentInteractionComponent::ServerBeginInteraction_Validate(const FString& Action, float Duration)
{
    return GetOwner() && GetOwner()->HasAuthority();
}

void UEnvironmentInteractionComponent::PushObject()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(TEXT("Push"));
        ServerInteract(Hit.GetActor(), TEXT("Push"));
    }
}

void UEnvironmentInteractionComponent::PullObject()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(TEXT("Pull"));
        ServerInteract(Hit.GetActor(), TEXT("Pull"));
    }
}

void UEnvironmentInteractionComponent::OpenDoor()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(TEXT("OpenDoor"));
        ServerInteract(Hit.GetActor(), TEXT("OpenDoor"));
    }
}

void UEnvironmentInteractionComponent::UseLever()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(TEXT("UseLever"));
        ServerInteract(Hit.GetActor(), TEXT("UseLever"));
    }
}

void UEnvironmentInteractionComponent::GrabLedge()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        if (Hit.GetActor()->Implements<UGameplayTagAssetInterface>())
        {
            IGameplayTagAssetInterface* TagInterface = Cast<IGameplayTagAssetInterface>(Hit.GetActor());
            if (TagInterface && TagInterface->HasMatchingGameplayTag(LedgeTag))
            {
                BeginInteraction(TEXT("GrabLedge"));
                ServerInteract(Hit.GetActor(), TEXT("GrabLedge"));
            }
        }
    }
}

void UEnvironmentInteractionComponent::UseZipline()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(TEXT("UseZipline"));
        ServerInteract(Hit.GetActor(), TEXT("UseZipline"));
    }
}

void UEnvironmentInteractionComponent::UseAction()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (!Hit.GetActor())
    {
        return;
    }

    if (Hit.GetActor()->ActorHasTag(TEXT("Door")))
    {
        OpenDoor();
    }
    else if (Hit.GetActor()->ActorHasTag(TEXT("Lever")))
    {
        UseLever();
    }
    else if (Hit.GetActor()->ActorHasTag(TEXT("Pushable")))
    {
        PushObject();
    }
}

void UEnvironmentInteractionComponent::ServerInteract_Implementation(AActor* Target, const FString& Action)
{
    InteractedActor = Target;
    LastAction = Action;

    HandleInteraction(Target, Action);
    MulticastInteract(Target, Action);
}

bool UEnvironmentInteractionComponent::ServerInteract_Validate(AActor* Target, const FString& Action)
{
    return GetOwner() && GetOwner()->HasAuthority() && Target != nullptr;
}

void UEnvironmentInteractionComponent::HandleInteraction(AActor* Target, const FString& Action)
{
    if (!Target)
    {
        return;
    }

    Target->SetReplicates(true);
    if (!Target->GetReplicateMovement())
    {
        UE_LOG(LogTemp, Warning, TEXT("Interactive actor %s missing bReplicateMovement. Enabling replication."), *Target->GetName());
        Target->SetReplicateMovement(true);
    }

    UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(Target->GetRootComponent());

    if (Action == TEXT("Push"))
    {
        if (RootPrim && RootPrim->IsSimulatingPhysics())
        {
            RootPrim->SetPhysicsLinearVelocity(Target->GetActorForwardVector() * 200.f);
        }
        else
        {
            Target->AddActorWorldOffset(Target->GetActorForwardVector() * 50.f);
        }
    }
    else if (Action == TEXT("Pull"))
    {
        if (RootPrim && RootPrim->IsSimulatingPhysics())
        {
            RootPrim->SetPhysicsLinearVelocity(-Target->GetActorForwardVector() * 200.f);
        }
        else
        {
            Target->AddActorWorldOffset(-Target->GetActorForwardVector() * 50.f);
        }
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
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn && CachedMovement)
        {
            FVector TargetLocation = Target->GetActorLocation() + Target->GetActorForwardVector() * 50.f + FVector(0.f, 0.f, 100.f);
            CachedMovement->SetMovementMode(MOVE_Flying);
            OwnerPawn->SetActorLocation(TargetLocation);
            CachedMovement->SetMovementMode(MOVE_Walking);
        }
    }
    else if (Action == TEXT("UseZipline"))
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn && CachedMovement)
        {
            USplineComponent* Spline = Target->FindComponentByClass<USplineComponent>();
            if (!Spline)
            {
                if (UCableComponent* Cable = Target->FindComponentByClass<UCableComponent>())
                {
                    Spline = Cable->GetAttachEndToComponent() ? Cable->GetAttachEndToComponent()->FindComponentByClass<USplineComponent>() : nullptr;
                }
            }

            if (Spline)
            {
                ActiveZipline = Spline;
                ZiplineProgress = 0.f;
                CachedMovement->SetMovementMode(MOVE_Flying);
                OwnerPawn->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
                GetWorld()->GetTimerManager().SetTimer(ZiplineTimerHandle, this, &UEnvironmentInteractionComponent::UpdateZiplineMovement, 0.02f, true);
            }
        }
    }
}

void UEnvironmentInteractionComponent::MulticastInteract_Implementation(AActor* Target, const FString& Action)
{
    if (GetOwnerRole() == ROLE_SimulatedProxy)
    {
        HandleInteraction(Target, Action);
    }
}

void UEnvironmentInteractionComponent::OnRep_Interaction()
{
    HandleInteraction(InteractedActor, LastAction);
}

void UEnvironmentInteractionComponent::UpdateZiplineMovement()
{
    if (!ActiveZipline)
    {
        StopZipline();
        return;
    }

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn)
    {
        StopZipline();
        return;
    }

    ZiplineProgress += ZiplineSpeed * 0.02f;
    float SplineLength = ActiveZipline->GetSplineLength();
    FVector NewLocation = ActiveZipline->GetLocationAtDistanceAlongSpline(ZiplineProgress, ESplineCoordinateSpace::World);
    OwnerPawn->SetActorLocation(NewLocation);

    if (ZiplineProgress >= SplineLength)
    {
        StopZipline();
    }
}

void UEnvironmentInteractionComponent::StopZipline()
{
    GetWorld()->GetTimerManager().ClearTimer(ZiplineTimerHandle);

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (OwnerPawn)
    {
        OwnerPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    if (CachedMovement)
    {
        CachedMovement->SetMovementMode(MOVE_Walking);
    }

    ActiveZipline = nullptr;
    ZiplineProgress = 0.f;
}

