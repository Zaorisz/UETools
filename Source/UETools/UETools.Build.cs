// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UETools : ModuleRules
{
	public UETools(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "CableComponent" });

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}