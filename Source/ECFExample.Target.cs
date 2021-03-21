// Copyright (c) 2021 Damian Nowakowski. All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ECFExampleTarget : TargetRules
{
	public ECFExampleTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "ECFExample" } );
	}
}
