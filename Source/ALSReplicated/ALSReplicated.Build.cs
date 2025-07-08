// Some copyright should be here...

using UnrealBuildTool;

public class ALSReplicated : ModuleRules
{
	public ALSReplicated(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
                PublicIncludePaths.AddRange(
                        new string[] {
                                System.IO.Path.Combine(ModuleDirectory, "Public"),
                                System.IO.Path.Combine(ModuleDirectory, "Public/Camera"),
                                System.IO.Path.Combine(ModuleDirectory, "Public/Environment"),
                                System.IO.Path.Combine(ModuleDirectory, "Public/UI")
                        }
                        );
				
		
                PrivateIncludePaths.AddRange(
                        new string[] {
                                System.IO.Path.Combine(ModuleDirectory, "Private"),
                                System.IO.Path.Combine(ModuleDirectory, "Private/Camera"),
                                System.IO.Path.Combine(ModuleDirectory, "Private/Environment"),
                                System.IO.Path.Combine(ModuleDirectory, "Private/UI")
                        }
                        );
			
		
               PublicDependencyModuleNames.AddRange(
                       new string[]
                       {
                               "Core",
                               "UMG",
                               "AIModule",
                               "GameplayTasks",
                               "GameplayTags",
                               "CableComponent",
                               "Json",
                               "JsonUtilities",
                               // ... add other public dependencies that you statically link with here ...
                       }
                       );
			
		
                PrivateDependencyModuleNames.AddRange(
                        new string[]
                        {
                                "Core",
                                "CoreUObject",
                                "Engine",
                                "Slate",
                                "SlateCore",
                                "UMG",
                                "AIModule",
                                "GameplayTasks",
                                "GameplayTags",
                                "NavigationSystem",
                                "Niagara",
                               "CableComponent",
                               "Json",
                               "JsonUtilities",
                               // ... add private dependencies that you statically link with here ...
                        }
                        );
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
