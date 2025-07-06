#include "Misc/AutomationTest.h"
#include "LockOnComponent.h"
#include "EnvironmentInteractionComponent.h"
#include "HitReactionComponent.h"

#if WITH_DEV_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLockOnReplicationFlagsTest, "ALSReplicated.ComponentReplication.LockOnReplicationFlags", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FLockOnReplicationFlagsTest::RunTest(const FString& Parameters)
{
    UClass* LockOnClass = ULockOnComponent::StaticClass();

    FProperty* LockedProp = LockOnClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ULockOnComponent, LockedTargetId));
    TestTrue(TEXT("LockedTargetId should replicate"), LockedProp && LockedProp->HasAnyPropertyFlags(CPF_Net));

    FProperty* TickActiveProp = LockOnClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ULockOnComponent, bTickActive));
    TestTrue(TEXT("bTickActive should replicate"), TickActiveProp && TickActiveProp->HasAnyPropertyFlags(CPF_Net));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEnvironmentInteractionReplicationFlagsTest, "ALSReplicated.ComponentReplication.EnvironmentInteractionReplicationFlags", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FEnvironmentInteractionReplicationFlagsTest::RunTest(const FString& Parameters)
{
    UClass* EnvClass = UEnvironmentInteractionComponent::StaticClass();

    FProperty* InteractedProp = EnvClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UEnvironmentInteractionComponent, InteractedActor));
    TestTrue(TEXT("InteractedActor should replicate"), InteractedProp && InteractedProp->HasAnyPropertyFlags(CPF_Net));

    FProperty* LastActionProp = EnvClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UEnvironmentInteractionComponent, LastAction));
    TestTrue(TEXT("LastAction should replicate"), LastActionProp && LastActionProp->HasAnyPropertyFlags(CPF_Net));

    FProperty* InteractingProp = EnvClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UEnvironmentInteractionComponent, bIsInteracting));
    TestTrue(TEXT("bIsInteracting should replicate"), InteractingProp && InteractingProp->HasAnyPropertyFlags(CPF_Net));

    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHitReactionReplicationFlagsTest, "ALSReplicated.ComponentReplication.HitReactionReplicationFlags", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FHitReactionReplicationFlagsTest::RunTest(const FString& Parameters)
{
    UClass* HitReactionClass = UHitReactionComponent::StaticClass();

    FProperty* KnockedOutProp = HitReactionClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UHitReactionComponent, bIsKnockedOut));
    TestTrue(TEXT("bIsKnockedOut should replicate"), KnockedOutProp && KnockedOutProp->HasAnyPropertyFlags(CPF_Net));

    FProperty* StaminaProp = HitReactionClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UHitReactionComponent, Stamina));
    TestTrue(TEXT("Stamina should replicate"), StaminaProp && StaminaProp->HasAnyPropertyFlags(CPF_Net));

    FProperty* RagdollProp = HitReactionClass->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UHitReactionComponent, bIsRagdoll));
    TestTrue(TEXT("bIsRagdoll should replicate"), RagdollProp && RagdollProp->HasAnyPropertyFlags(CPF_Net));

    return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS

