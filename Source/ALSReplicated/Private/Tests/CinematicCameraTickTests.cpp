#include "Misc/AutomationTest.h"
#include "Camera/CinematicCameraComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCinematicCameraTickTest, "ALSReplicated.Camera.Cinematic.TickRuns", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FCinematicCameraTickTest::RunTest(const FString& Parameters)
{
    UCinematicCameraComponent* Camera = NewObject<UCinematicCameraComponent>();
    TestNotNull(TEXT("Camera component should be created"), Camera);
    if (!Camera)
    {
        return false;
    }

    Camera->TickComponent(0.016f, LEVELTICK_TimeOnly, nullptr);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
