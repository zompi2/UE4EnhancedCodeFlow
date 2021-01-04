// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

using UnrealBuildTool;

public class EnhancedCodeFlow : ModuleRules
{
	public EnhancedCodeFlow(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
		);
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine"
			}
		);
	}
}
