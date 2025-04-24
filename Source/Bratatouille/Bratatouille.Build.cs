// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Bratatouille : ModuleRules
{
	public Bratatouille(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG","LevelSequence", "MovieScene" });

		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "NavigationSystem" });
	}
}
