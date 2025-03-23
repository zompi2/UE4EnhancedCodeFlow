// Copyright (c) 2025 Damian Nowakowski. All rights reserved.

using UnrealBuildTool;

public class EnhancedCodeFlow : ModuleRules
{
	public EnhancedCodeFlow(ReadOnlyTargetRules Target) : base(Target)
	{
        // To ensure IWYU
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        // Ensure the default cpp standard is supported (CPP20 for UE5.3 and above)
        CppStandard = CppStandardVersion.Default;

        // Required modules
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

        // Allow to disable optimization for non shipping builds (for easier debugging)
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

        // Enable or disable logs displayed 
        bool bShowLogs = true;
        bool bShowVerboseLogs = true;
        if (bShowLogs)
        {
            PublicDefinitions.Add("ECF_LOGS=1");
            if (bShowVerboseLogs)
            {
                PublicDefinitions.Add("ECF_LOGS_VERBOSE=1");
            }
            else
            {
                PublicDefinitions.Add("ECF_LOGS_VERBOSE=0");
            }
        }
        else
        {
            PublicDefinitions.Add("ECF_LOGS=0");
            PublicDefinitions.Add("ECF_LOGS_VERBOSE=0");
        }
    }
}
