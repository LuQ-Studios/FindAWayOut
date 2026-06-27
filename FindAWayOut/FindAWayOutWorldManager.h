// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PulseSaveGame.h"
#include "Interfaces/PulseSaveInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FindAWayOutWorldManager.generated.h"

class AFindAWayOutGameMode;
class UPulseSaveManager;
class UFindAWayOutWorldManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FFindAWayOutWorldChangedSignature, UFindAWayOutWorldManager*);

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutWorldManager : public UGameInstanceSubsystem, public IPulseSaveInterface
{
	GENERATED_BODY()
public:
	
	static const FName SaveID;
	
public:
	
	static UFindAWayOutWorldManager& Get(const UObject* WorldContextObject);
	
public:
	
	// ~Begin UGameInstanceSubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~End UGameInstanceSubsystem
	
public:
	
	// ~Begin IPulseSaveInterface
	virtual void OnLoad_Implementation(UPulseSaveManager* SaveManager, UPulseSaveModel* SaveModel) override;
	virtual EPulseSaveChoice IsReadyToLoad_Implementation(UPulseSaveManager* SaveManager, const UPulseSaveModel* SaveModel) const override;
	virtual FName GetUniqueSaveIdentifier_Implementation() const override { return SaveID; }
	virtual TSubclassOf<UPulseSaveModel> GetSaveModelClass_Implementation(UPulseSaveManager* SaveManager) const override;
	// ~End IPulseSaveInterface
	
public:
	
	/** Triggered when the world has changed. */
	FFindAWayOutWorldChangedSignature OnWorldChanged;
	
public:
	
	/**
	 * Called by the game inside InitGame to notify the world manager the world has changed.
	 * @param GameMode - The new game mode of the world.
	 */
	void NotifyWorldChanged(AFindAWayOutGameMode* GameMode);
	
private:
	
	/** Called when a new world save game is created */
	void OnNewWorldSaveGameCreated(FPulseSaveGame NewSaveGame);
	
private:
	
	/** Requested world tag to change to. */
	FGameplayTag RequestedWorldTag;
	
	/** Called when a save is complete */
	void OnWorldSaveComplete_ChangeWorld(UPulseSaveManager* SaveManager, bool bSuccess);
	
	/** Loads a new world */
	bool LoadWorldByTag(const FGameplayTag& WorldTag);
	
public:
	
	/** Current tag of the current world */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|World")
	FGameplayTag CurrentWorldTag;
	
public:
	
	/**
	 * Requests a world change.
	 * This will save the current world and then switches to the new world.
	 * @param WorldTag - The tag of the world to change to.
	 * @param bSaveCurrentWorld - Whether to save the current world before changing.
	 * @returns true if the world change was successfully requested, false otherwise.
	 */
	UFUNCTION(BlueprintCallable, Category = "Find A Way Out|World")
	bool RequestWorldChange(FGameplayTag WorldTag, bool bSaveCurrentWorld = true);
};