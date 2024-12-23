// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Projet_AI : ModuleRules
{
	public Projet_AI(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "CookingFactory", "GameplayTags" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "NavigationSystem", "CookingFactory" });
	}
}
