// Copyright 2021 Dark Point Games sp. z o.o. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FlowTestTarget : TargetRules
{
	public FlowTestTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "FlowTest" } );
	}
}
