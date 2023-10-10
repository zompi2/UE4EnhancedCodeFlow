// Copyright (c) 2023 Damian Nowakowski. All rights reserved.

using UnrealBuildTool;

public class EnhancedCodeFlow : ModuleRules
{
	public EnhancedCodeFlow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // Ensure we use Cpp20 for UE5.3 and above.
        bool bUseCpp20IfPossible = true;
        if (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 3)
        {
            if (bUseCpp20IfPossible)
            {
                CppStandard = CppStandardVersion.Cpp20;
            }
        }

        PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

        // Ensure there are no duplicated definitions already
        PublicDefinitions.RemoveAll(ECFDefinition => ECFDefinition.StartsWith("ECF_"));

        // Disable optimization for non shipping builds (for easier debugging)
        bool bDisableOptimization = false;
        if (bDisableOptimization && (Target.Configuration != UnrealTargetConfiguration.Shipping))
        {
            if (Target.Version.MajorVersion == 5 && Target.Version.MinorVersion >= 2)
            {
                PublicDefinitions.Add("ECF_PRAGMA_DISABLE_OPTIMIZATION=UE_DISABLE_OPTIMIZATION");
                PublicDefinitions.Add("ECF_PRAGMA_ENABLE_OPTIMIZATION=UE_ENABLE_OPTIMIZATION");
            }
            else
            {
                PublicDefinitions.Add("ECF_PRAGMA_DISABLE_OPTIMIZATION=PRAGMA_DISABLE_OPTIMIZATION");
                PublicDefinitions.Add("ECF_PRAGMA_ENABLE_OPTIMIZATION=PRAGMA_ENABLE_OPTIMIZATION");
            }
        }
        else
        {
            PublicDefinitions.Add("ECF_PRAGMA_DISABLE_OPTIMIZATION=");
            PublicDefinitions.Add("ECF_PRAGMA_ENABLE_OPTIMIZATION=");
        }

        // Enable or disable extra traces for Unreal Insight profiler
        bool bEnableInsightProfiling = true;
        if (bEnableInsightProfiling)
        {
            PublicDefinitions.Add("ECF_INSIGHT_PROFILING=1");
        }
        else
        {
            PublicDefinitions.Add("ECF_INSIGHT_PROFILING=0");
        }
    }
}
