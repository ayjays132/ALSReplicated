#include "Misc/AutomationTest.h"
#include "AI/EmotionStateComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEmotionStateReplicationTest, "ALSReplicated.EmotionState.Replication", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FEmotionStateReplicationTest::RunTest(const FString& Parameters)
{
    UClass* EmotionClass = UEmotionStateComponent::StaticClass();
    FProperty* Prop = EmotionClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UEmotionStateComponent, CurrentEmotion));
    const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
    TestTrue(TEXT("CurrentEmotion should replicate"), bReplicated);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
