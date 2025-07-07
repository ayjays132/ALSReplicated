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

    class UTestStanceManagerComponent : public UStanceManagerComponent
    {
    public:
        using UStanceManagerComponent::OnRep_Stance;
    };

    UTestStanceManagerComponent* Comp = NewObject<UTestStanceManagerComponent>();
    int32 ChangeCount = 0;
    EStanceMode LastStance = EStanceMode::Normal;
    Comp->OnStanceChanged.AddLambda([&](EStanceMode NewStance)
    {
        ++ChangeCount;
        LastStance = NewStance;
    });

    Comp->CurrentStance = EStanceMode::Aggressive;
    Comp->OnRep_Stance();

    TestEqual(TEXT("OnStanceChanged fired via OnRep_Stance"), ChangeCount, 1);
    TestEqual(TEXT("New stance replicated"), LastStance, EStanceMode::Aggressive);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
