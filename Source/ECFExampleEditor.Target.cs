// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ECFExampleEditorTarget : TargetRules
{
	public ECFExampleEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ECFExample" } );
	}
}
