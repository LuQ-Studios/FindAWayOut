// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "LoadingProcessInterface.h"
#include "PulseSaveGame.h"
#include "PulseSaveManager.h"
#include "Interfaces/PulseSaveInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FindAWayOutSaveSubsystem.generated.h"


class UFindAWayOutWorldSaveCollector;
class UFindAWayOutGameSaveCollector;
/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutSaveSubsystem : public UGameInstanceSubsystem, public IPulseSaveInterface, public ILoadingProcessInterface
{
	GENERATED_BODY()
public:
	
	static const FName SaveID;
	static UFindAWayOutSaveSubsystem& Get(const UObject* WorldContextObject);
	
public:
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
public:
	
	virtual FName GetUniqueSaveIdentifier_Implementation() const override;
	virtual TSubclassOf<UPulseSaveModel> GetSaveModelClass_Implementation(UPulseSaveManager* SaveManager) const override;
	
public:
	
	virtual bool ShouldShowLoadingScreen(FString& OutReason) const override;
	
public:
	
	/** World saves mapped to their world tag */
	TMap<FGameplayTag, FPulseSaveGame> WorldSaves;
	
public:
	
	/** Sets the current game save. */
	void SetWorldSaveGame(const FPulseWeakSaveGame& NewWorldSave);
	
	/** Loads the current world save into the world. */
	void LoadCurrentWorldSave();
	
	/** Saves the current world */
	bool SaveCurrentWorld(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback = FPulseSaveManagerProcessedSignature::FDelegate());
	
	/** Loads the current world */
	bool LoadCurrentWorld(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback = FPulseSaveManagerProcessedSignature::FDelegate());
	
private:
	
	void InternalSetGameSave(const FPulseSaveGame& NewSaveGame, const FString& SlotName, bool bLoadSaveGame = true, const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback = FPulseSaveManagerProcessedSignature::FDelegate());
	
	/** Called when a new game is created */
	void OnNewGameCreated();
	
public:
	
	/** Current save game slot */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|Save")
	FString CurrentSaveSlot;
	
	/** Current loaded game save */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|Save")
	FPulseStrongSaveGame CurrentGameSave;
	
	/** Current world save game */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|Save")
	FPulseStrongSaveGame CurrentWorldSave;
	
	/** Save manager instance */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|Save")
	TObjectPtr<UPulseSaveManager> GameSaveManager;
	
	/** World save manager instance */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|Save")
	TObjectPtr<UPulseSaveManager> WorldSaveManager;
	
	/** Game save collector */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|Save")
	TObjectPtr<UFindAWayOutGameSaveCollector> GameSaveCollector;
	
	/** World save collector */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Find A Way Out|Save")
	TObjectPtr<UFindAWayOutWorldSaveCollector> WorldSaveCollector;
	
public:
	
	/** Saves the current world */
	UFUNCTION(BlueprintCallable, Category = "Find A Way Out|Save")
	bool SaveWorld();
	
	/** Loads the current world */
	UFUNCTION(BlueprintCallable, Category = "Find A Way Out|Save")
	bool LoadWorld();
	
	/** Checks if the game or world is saving/loading */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Find A Way Out|Save")
	bool IsSavingOrLoading() const;
	
	/** Checks if the game is saving/loading */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Find A Way Out|Save")
	bool IsGameSavingOrLoading() const;
	
	/** Checks if the world is saving/loading */
	UFUNCTION(BlueprintCallable, Category = "Find A Way Out|Save")
	bool IsWorldSavingOrLoading() const;
	
public:
	
	/** 
	 * Creates a game 
	 * @param SlotName - The name of the save slot to create the new game in.
	 * @returns Whether the new game was successfully created. This will return false if a save already exists in the specified slot.
	 */
	UFUNCTION(BlueprintCallable, Category = "Find A Way Out|Save")
	bool CreateNewGame(const FString& SlotName);
	
	/**
	 * Saves the current save game.
	 * @returns Whether the save process was successfully started.
	 */
	UFUNCTION(BlueprintCallable, Category = "Find A Way Out|Save")
	bool SaveGame();
	bool SaveCurrentGame(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback = FPulseSaveManagerProcessedSignature::FDelegate());
	
	/**
	 * Loads a game save from the specified slot and sets it as the current game save.
	 * @param SlotName - The name of the save slot to load the game from.
	 * @returns Whether the load process was successfully started. This will return false if the specified
	 */
	UFUNCTION(BlueprintCallable, Category = "Find A Way Out|Save")
	bool LoadGame(const FString& SlotName);
	bool LoadCurrentSaveGame(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback);
	
	/**
	 * Gets a list of all existing save slots.
	 * @param OutSaveSlots - An array that will be filled with the names of all existing save slots.
	 * @returns Whether the save slots were successfully retrieved.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Find A Way Out|Save")
	bool GetExistingSaveSlots(TArray<FString>& OutSaveSlots) const;
};
