using UnrealBuildTool;
using System.Collections.Generic;

public class ALSReplicatedServerTarget : TargetRules
{
    public ALSReplicatedServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.Add("ALSReplicated");
    }
}
