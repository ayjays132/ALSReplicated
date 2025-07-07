#include "Misc/AutomationTest.h"
#include "ImpactEventSubsystem.h"
#include "HitReactionComponent.h"
#include "GameFramework/Actor.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FImpactSubsystemBroadcastTest, "ALSReplicated.ImpactSubsystem.Broadcast", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FImpactSubsystemBroadcastTest::RunTest(const FString& Parameters)
{
    AActor* Owner = NewObject<AActor>();

    class UTestHitReactionComponent : public UHitReactionComponent
    {
    public:
        UImpactEventSubsystem* Subsystem = nullptr;
        using UHitReactionComponent::ReceiveHit;
        using UHitReactionComponent::PlayHit;
    protected:
        void PlayHit(FVector HitLocation, FVector HitDirection, bool bCriticalHit, bool bBlocked, bool bKnockout)
        {
            UHitReactionComponent::PlayHit(HitLocation, HitDirection, bCriticalHit, bBlocked, bKnockout);
            if (Subsystem)
            {
                Subsystem->BroadcastImpact(GetOwner(), HitLocation);
            }
        }
    };

    UImpactEventSubsystem* Subsystem = NewObject<UImpactEventSubsystem>();
    UTestHitReactionComponent* Comp = NewObject<UTestHitReactionComponent>(Owner);
    Comp->Subsystem = Subsystem;
    Comp->RegisterComponent();

    int32 BroadcastCount = 0;
    Subsystem->OnHardImpact.AddLambda([&](AActor*, FVector){ ++BroadcastCount; });

    Comp->ReceiveHit(FVector::ZeroVector, FVector::ForwardVector, false, false, false);

    TestEqual(TEXT("Hard impact broadcast"), BroadcastCount, 1);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
