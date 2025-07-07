#include "Misc/AutomationTest.h"
#include "Camera/CinematicCameraComponent.h"
#include "CharacterStateCoordinator.h"
#include "StaminaComponent.h"
#include "GameFramework/Actor.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCinematicCameraFOVChangeTest, "ALSReplicated.Camera.Cinematic.FOVChangesInCombat", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FCinematicCameraFOVChangeTest::RunTest(const FString& Parameters)
{
    AActor* Owner = NewObject<AActor>();
    UCinematicCameraComponent* Camera = NewObject<UCinematicCameraComponent>(Owner);
    UCharacterStateCoordinator* State = NewObject<UCharacterStateCoordinator>(Owner);
    UStaminaComponent* Stamina = NewObject<UStaminaComponent>(Owner);

    Camera->RegisterComponent();
    State->RegisterComponent();
    Stamina->RegisterComponent();

    State->SetCharacterState(ECharacterActivityState::Combat);

    float LastFOV = 0.f;
    Camera->OnFOVChanged.AddLambda([&](float NewFOV) { LastFOV = NewFOV; });

    Camera->TickComponent(0.016f, LEVELTICK_TimeOnly, nullptr);

    TestTrue(TEXT("FOV should change when combat state is active"), LastFOV > 0.f && LastFOV <= Camera->DefaultFOV);
    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
