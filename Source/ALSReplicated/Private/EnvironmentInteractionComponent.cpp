#include "EnvironmentInteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameplayTagAssetInterface.h"
#include "Components/SplineComponent.h"
#include "CableComponent.h"
#include "Components/PrimitiveComponent.h"
#include "TraversalSmartRouter.h"
#include "VisualImpactSystem.h"
#include "ImpactEventSubsystem.h"

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
        TraversalRouter = OwnerActor->FindComponentByClass<UTraversalSmartRouter>();
    }
}

void UEnvironmentInteractionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UEnvironmentInteractionComponent, InteractionInfo);
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

void UEnvironmentInteractionComponent::BeginInteraction(EInteractionAction Action, float Duration)
{
    if (GetOwnerRole() < ROLE_Authority)
    {
        ServerBeginInteraction(Action, Duration);
    }

    bIsInteracting = true;
    InteractionInfo.Action = Action;

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

void UEnvironmentInteractionComponent::ServerBeginInteraction_Implementation(EInteractionAction Action, float Duration)
{
    BeginInteraction(Action, Duration);
}

bool UEnvironmentInteractionComponent::ServerBeginInteraction_Validate(EInteractionAction Action, float Duration)
{
    return GetOwner() && GetOwner()->HasAuthority();
}

void UEnvironmentInteractionComponent::PushObject()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(EInteractionAction::Push);
        ServerInteract(Hit.GetActor(), EInteractionAction::Push);
    }
}

void UEnvironmentInteractionComponent::PullObject()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(EInteractionAction::Pull);
        ServerInteract(Hit.GetActor(), EInteractionAction::Pull);
    }
}

void UEnvironmentInteractionComponent::OpenDoor()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(EInteractionAction::OpenDoor);
        ServerInteract(Hit.GetActor(), EInteractionAction::OpenDoor);
    }
}

void UEnvironmentInteractionComponent::UseLever()
{
    FHitResult Hit;
    PerformTrace(Hit);
    if (Hit.GetActor())
    {
        BeginInteraction(EInteractionAction::UseLever);
        ServerInteract(Hit.GetActor(), EInteractionAction::UseLever);
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
                BeginInteraction(EInteractionAction::GrabLedge);
                ServerInteract(Hit.GetActor(), EInteractionAction::GrabLedge);
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
        BeginInteraction(EInteractionAction::UseZipline);
        ServerInteract(Hit.GetActor(), EInteractionAction::UseZipline);
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
    else if (TraversalRouter)
    {
        const ETraversalAction Action = TraversalRouter->EvaluateTraversal();
        switch (Action)
        {
        case ETraversalAction::LedgeGrab:
        case ETraversalAction::Mantle:
        case ETraversalAction::WallClimb:
            GrabLedge();
            break;
        case ETraversalAction::Zipline:
            UseZipline();
            break;
        default:
            break;
        }
    }
}

void UEnvironmentInteractionComponent::ServerInteract_Implementation(AActor* Target, EInteractionAction Action)
{
    InteractionInfo.Actor = Target;
    InteractionInfo.Action = Action;

    HandleInteraction(Target, Action);
    MulticastInteract(Target, Action);
}

bool UEnvironmentInteractionComponent::ServerInteract_Validate(AActor* Target, EInteractionAction Action)
{
    return GetOwner() && GetOwner()->HasAuthority() && Target != nullptr;
}

void UEnvironmentInteractionComponent::HandleInteraction(AActor* Target, EInteractionAction Action)
{
    if (!Target)
    {
        return;
    }

    if (!Target->GetIsReplicated())
    {
        UE_LOG(LogTemp, Warning, TEXT("Interactive actor %s is not set to replicate."), *Target->GetName());
    }

    if (!Target->GetReplicateMovement())
    {
        UE_LOG(LogTemp, Warning, TEXT("Interactive actor %s missing bReplicateMovement."), *Target->GetName());
    }

    UPrimitiveComponent* RootPrim = Cast<UPrimitiveComponent>(Target->GetRootComponent());

    switch (Action)
    {
    case EInteractionAction::Push:
        if (RootPrim && RootPrim->IsSimulatingPhysics())
        {
            RootPrim->SetPhysicsLinearVelocity(Target->GetActorForwardVector() * 200.f);
        }
        else
        {
            Target->AddActorWorldOffset(Target->GetActorForwardVector() * 50.f);
        }
        break;
    case EInteractionAction::Pull:
        if (RootPrim && RootPrim->IsSimulatingPhysics())
        {
            RootPrim->SetPhysicsLinearVelocity(-Target->GetActorForwardVector() * 200.f);
        }
        else
        {
            Target->AddActorWorldOffset(-Target->GetActorForwardVector() * 50.f);
        }
        break;
    case EInteractionAction::OpenDoor:
        Target->AddActorWorldRotation(FRotator(0.f, 90.f, 0.f));
        break;
    case EInteractionAction::UseLever:
        Target->AddActorWorldRotation(FRotator(-45.f, 0.f, 0.f));
        break;
    case EInteractionAction::GrabLedge:
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn && CachedMovement)
        {
            FVector TargetLocation = Target->GetActorLocation() + Target->GetActorForwardVector() * 50.f + FVector(0.f, 0.f, 100.f);
            CachedMovement->SetMovementMode(MOVE_Flying);
            OwnerPawn->SetActorLocation(TargetLocation);
            CachedMovement->SetMovementMode(MOVE_Walking);
        }
        break;
    }
    case EInteractionAction::UseZipline:
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
        break;
    }
    default:
        break;
    }

    if (InteractionFX || InteractionDecal)
    {
        FVector Location = Target->GetActorLocation();
        FVector Direction = (Location - GetOwner()->GetActorLocation()).GetSafeNormal();

        if (InteractionFX)
        {
            UVisualImpactSystem::SpawnDirectionalNiagaraFX(this, InteractionFX, Location, Direction, 1.f, HeavyImpactShake);
        }

        if (InteractionDecal)
        {
            UVisualImpactSystem::SpawnImpactDecal(this, InteractionDecal, Location, Direction, 1.f, InteractionDecalSize, InteractionDecalLifeSpan);
        }

        if (UWorld* World = GetWorld())
        {
            if (UImpactEventSubsystem* Subsystem = World->GetSubsystem<UImpactEventSubsystem>())
            {
                Subsystem->BroadcastImpact(GetOwner(), Location);
            }
        }
    }
}

void UEnvironmentInteractionComponent::MulticastInteract_Implementation(AActor* Target, EInteractionAction Action)
{
    if (GetOwnerRole() == ROLE_SimulatedProxy)
    {
        HandleInteraction(Target, Action);
    }
}

void UEnvironmentInteractionComponent::OnRep_Interaction()
{
    HandleInteraction(InteractionInfo.Actor, InteractionInfo.Action);
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

