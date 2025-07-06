#include "Misc/AutomationTest.h"
#include "CharacterStateCoordinator.h"
#include "StaminaComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterStateCoordinatorEventsTest, "ALSReplicated.CharacterStateCoordinator.Events", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FCharacterStateCoordinatorEventsTest::RunTest(const FString& Parameters)
{
    UCharacterStateCoordinator* Coordinator = NewObject<UCharacterStateCoordinator>();
    int32 CombatCount = 0;
    int32 CriticalCount = 0;
    int32 TraverseCount = 0;

    Coordinator->OnCombatEngaged.AddLambda([&]() { ++CombatCount; });
    Coordinator->OnStaminaCritical.AddLambda([&]() { ++CriticalCount; });
    Coordinator->OnTraversalAction.AddLambda([&]() { ++TraverseCount; });

    Coordinator->SetCharacterState(ECharacterActivityState::Combat);
    TestEqual(TEXT("OnCombatEngaged fired"), CombatCount, 1);

    UStaminaComponent* Stamina = NewObject<UStaminaComponent>();
    Stamina->OnStaminaDepleted.AddDynamic(Coordinator, &UCharacterStateCoordinator::HandleStaminaDepleted);
    Stamina->OnStaminaDepleted.Broadcast();
    TestEqual(TEXT("OnStaminaCritical fired"), CriticalCount, 1);

    Coordinator->NotifyTraversalAction();
    TestEqual(TEXT("OnTraversalAction fired"), TraverseCount, 1);

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
