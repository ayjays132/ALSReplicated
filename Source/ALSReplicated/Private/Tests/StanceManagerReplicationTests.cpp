#include "Misc/AutomationTest.h"
#include "StanceManagerComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStanceManagerCurrentStanceReplicationTest, "ALSReplicated.ComponentReplication.StanceManager.CurrentStance", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FStanceManagerCurrentStanceReplicationTest::RunTest(const FString& Parameters)
{
    UClass* StanceClass = UStanceManagerComponent::StaticClass();
    FProperty* Prop = StanceClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UStanceManagerComponent, CurrentStance));
    const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
    TestTrue(TEXT("CurrentStance should replicate"), bReplicated);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
