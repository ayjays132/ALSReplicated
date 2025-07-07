#include "Misc/AutomationTest.h"
#include "ALSBaseCharacter.h"
#include "CombatComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

class UTestCombatComponent : public UCombatComponent
{
public:
    using UCombatComponent::CachedMovement;
    void PerformDashNoTimer()
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (!OwnerCharacter || !CachedMovement || !bUseDashRootMotion)
        {
            return;
        }
        UAnimInstance* AnimInst = OwnerCharacter->GetMesh() ? OwnerCharacter->GetMesh()->GetAnimInstance() : nullptr;
        if (!AnimInst)
        {
            return;
        }
        PrevMode = CachedMovement->MovementMode;
        PrevRoot = AnimInst->GetRootMotionMode();
        AnimInst->SetRootMotionMode(ERootMotionMode::RootMotionFromMontagesOnly);
        CachedMovement->SetMovementMode(MOVE_Flying);
    }
    EMovementMode PrevMode = MOVE_Walking;
    ERootMotionMode::Type PrevRoot = ERootMotionMode::IgnoreRootMotion;
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDashRootMotionStateActivationTest, "ALSReplicated.Combat.DashRootMotionState", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDashRootMotionStateActivationTest::RunTest(const FString& Parameters)
{
    AALSBaseCharacter* ServerChar = NewObject<AALSBaseCharacter>();
    ServerChar->GetMesh()->SetAnimInstanceClass(UAnimInstance::StaticClass());
    ServerChar->ALSCharacterMovement->RegisterComponent();
    ServerChar->GetMesh()->RegisterComponent();

    UTestCombatComponent* ServerComp = NewObject<UTestCombatComponent>(ServerChar);
    ServerComp->DashMontage = NewObject<UAnimMontage>(ServerComp);
    ServerComp->bUseDashRootMotion = true;
    ServerComp->CachedMovement = ServerChar->ALSCharacterMovement;
    ServerComp->RegisterComponent();

    AALSBaseCharacter* ClientChar = NewObject<AALSBaseCharacter>();
    ClientChar->GetMesh()->SetAnimInstanceClass(UAnimInstance::StaticClass());
    ClientChar->ALSCharacterMovement->RegisterComponent();
    ClientChar->GetMesh()->RegisterComponent();

    UTestCombatComponent* ClientComp = NewObject<UTestCombatComponent>(ClientChar);
    ClientComp->DashMontage = ServerComp->DashMontage;
    ClientComp->bUseDashRootMotion = true;
    ClientComp->CachedMovement = ClientChar->ALSCharacterMovement;
    ClientComp->RegisterComponent();

    ServerComp->PerformDashNoTimer();
    ClientComp->PerformDashNoTimer();

    TestEqual(TEXT("Server root motion active"), ServerChar->GetMesh()->GetAnimInstance()->GetRootMotionMode(), ERootMotionMode::RootMotionFromMontagesOnly);
    TestEqual(TEXT("Client root motion active"), ClientChar->GetMesh()->GetAnimInstance()->GetRootMotionMode(), ERootMotionMode::RootMotionFromMontagesOnly);
    TestEqual(TEXT("Server movement mode set"), ServerChar->ALSCharacterMovement->MovementMode, MOVE_Flying);
    TestEqual(TEXT("Client movement mode set"), ClientChar->ALSCharacterMovement->MovementMode, MOVE_Flying);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
