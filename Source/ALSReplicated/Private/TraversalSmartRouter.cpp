#include "TraversalSmartRouter.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/SplineComponent.h"
#include "CableComponent.h"

UTraversalSmartRouter::UTraversalSmartRouter()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTraversalSmartRouter::BeginPlay()
{
    Super::BeginPlay();
}

void UTraversalSmartRouter::PerformTrace(FHitResult& Hit) const
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return;
    }
    FVector Start;
    FRotator Rot;
    OwnerActor->GetActorEyesViewPoint(Start, Rot);
    FVector End = Start + Rot.Vector() * TraceDistance;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwnerActor);
    GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
}

ETraversalAction UTraversalSmartRouter::EvaluateTraversal()
{
    FHitResult Hit;
    PerformTrace(Hit);
    ETraversalAction Chosen = ETraversalAction::None;

    if (Hit.GetActor())
    {
        // Zipline detection
        if (Hit.GetActor()->FindComponentByClass<USplineComponent>() ||
            Hit.GetActor()->FindComponentByClass<UCableComponent>())
        {
            Chosen = ETraversalAction::Zipline;
        }
        else
        {
            const float SlopeAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector)));
            if (SlopeAngle <= MaxMantleSlope)
            {
                Chosen = ETraversalAction::Mantle;
            }
            else if (SlopeAngle <= 70.f)
            {
                Chosen = ETraversalAction::LedgeGrab;
            }
            else
            {
                Chosen = ETraversalAction::WallClimb;
            }
        }
    }

    OnTraversalActionChosen.Broadcast(Chosen);
    return Chosen;
}

