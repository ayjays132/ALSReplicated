#include "Misc/AutomationTest.h"
#include "CombatComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCombatComponentReplicationTest, "ALSReplicated.ComponentReplication.CombatComponent", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FCombatComponentReplicationTest::RunTest(const FString& Parameters)
{
    // Ensure replicated flags are present on key properties
    UClass* CombatClass = UCombatComponent::StaticClass();

    {
        FProperty* Prop = CombatClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UCombatComponent, bIsAttacking));
        const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
        TestTrue(TEXT("bIsAttacking should replicate"), bReplicated);
    }

    {
        FProperty* Prop = CombatClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UCombatComponent, Stamina));
        const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
        TestTrue(TEXT("Stamina should replicate"), bReplicated);
    }

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
