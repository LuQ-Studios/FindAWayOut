// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FindAWayOut : ModuleRules
{
	public FindAWayOut(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"PulseGameCore",
			"PulseAbility",
			"PulseQuest",
			"PulseHUD",
			"GameplayAbilities",
			"CoreOnline",
			"GameplayTags",
			"PulseSave",
			"DeveloperSettings",
			"GameplayTasks",
			"CommonLoadingScreen"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"FindAWayOut",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
