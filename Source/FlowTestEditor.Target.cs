// Copyright 2021 Dark Point Games sp. z o.o. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FlowTestEditorTarget : TargetRules
{
	public FlowTestEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "FlowTest" } );
	}
}
