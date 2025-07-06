#include "Misc/AutomationTest.h"
#include "StaminaComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStaminaEventBroadcastTest, "ALSReplicated.Stamina.Events", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FStaminaEventBroadcastTest::RunTest(const FString& Parameters)
{
    UStaminaComponent* Stamina = NewObject<UStaminaComponent>();
    int32 DepletedCount = 0;
    int32 RecoveredCount = 0;
    Stamina->OnStaminaDepleted.AddLambda([&]() { ++DepletedCount; });
    Stamina->OnStaminaRecovered.AddLambda([&]() { ++RecoveredCount; });

    Stamina->AddStamina(-200.f);
    TestEqual(TEXT("OnStaminaDepleted fired once"), DepletedCount, 1);
    TestEqual(TEXT("OnStaminaRecovered not yet fired"), RecoveredCount, 0);

    Stamina->AddStamina(50.f);
    TestEqual(TEXT("OnStaminaRecovered fired once"), RecoveredCount, 1);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
