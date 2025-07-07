#include "Misc/AutomationTest.h"
#include "StaminaComponent.h"
#include "LockOnComponent.h"
#include "EnvironmentInteractionComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FStaminaReplicationTest, "ALSReplicated.StaminaReplication", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FStaminaReplicationTest::RunTest(const FString& Parameters)
{
    UClass* StaminaClass = UStaminaComponent::StaticClass();
    FProperty* Prop = StaminaClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UStaminaComponent, Stamina));
    const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
    TestTrue(TEXT("Stamina should replicate"), bReplicated);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMaxStaminaReplicationTest, "ALSReplicated.MaxStaminaReplication", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FMaxStaminaReplicationTest::RunTest(const FString& Parameters)
{
    UClass* StaminaClass = UStaminaComponent::StaticClass();
    FProperty* Prop = StaminaClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UStaminaComponent, MaxStamina));
    const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
    TestTrue(TEXT("MaxStamina should replicate"), bReplicated);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLockOnTargetReplicationTest, "ALSReplicated.LockOn.TargetReplication", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FLockOnTargetReplicationTest::RunTest(const FString& Parameters)
{
    UClass* LockOnClass = ULockOnComponent::StaticClass();
    FProperty* Prop = LockOnClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ULockOnComponent, LockedTargetId));
    const bool bReplicated = Prop && Prop->HasAnyPropertyFlags(CPF_Net);
    TestTrue(TEXT("LockedTargetId should replicate"), bReplicated);
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnvironmentInteractionReplicationTest, "ALSReplicated.EnvironmentInteractionReplication", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FEnvironmentInteractionReplicationTest::RunTest(const FString& Parameters)
{
    UClass* EnvClass = UEnvironmentInteractionComponent::StaticClass();

    FProperty* InteractionProp = EnvClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UEnvironmentInteractionComponent, InteractionInfo));
    TestTrue(TEXT("InteractionInfo should replicate"), InteractionProp && InteractionProp->HasAnyPropertyFlags(CPF_Net));

    FProperty* InteractingProp = EnvClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UEnvironmentInteractionComponent, bIsInteracting));
    TestTrue(TEXT("bIsInteracting should replicate"), InteractingProp && InteractingProp->HasAnyPropertyFlags(CPF_Net));

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
