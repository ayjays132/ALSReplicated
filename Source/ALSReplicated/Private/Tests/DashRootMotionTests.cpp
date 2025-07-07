#include "Misc/AutomationTest.h"
#include "CombatComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FDashRootMotionReplicationTest, "ALSReplicated.Combat.DashRootMotionReplication", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FDashRootMotionReplicationTest::RunTest(const FString& Parameters)
{
    UClass* CombatClass = UCombatComponent::StaticClass();
    FProperty* Prop = CombatClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UCombatComponent, bUseDashRootMotion));
    const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
    TestTrue(TEXT("bUseDashRootMotion should replicate"), bReplicated);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
