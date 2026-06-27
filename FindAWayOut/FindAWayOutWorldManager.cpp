// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutWorldManager.h"

#include "FindAWayOut.h"
#include "FindAWayOutGameMode.h"
#include "FindAWayOutSettings.h"
#include "Save/FindAWayOutSaveSubsystem.h"
#include "Save/Models/FindAWayOutWorldManagerSaveModel.h"
#include "World/FindAWayOutWorldDefinition.h"
#include "World/FindAWayOutWorldList.h"
#include "World/FindAWayOutWorldSettings.h"

const FName UFindAWayOutWorldManager::SaveID = TEXT("FindAWayOutWorldManager");

UFindAWayOutWorldManager& UFindAWayOutWorldManager::Get(const UObject* WorldContextObject)
{
	UWorld* world = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UFindAWayOutWorldManager* manager = UGameInstance::GetSubsystem<UFindAWayOutWorldManager>(world->GetGameInstance());
	
	check(manager);
	return *manager;
}

void UFindAWayOutWorldManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	// TODO: Bind online delegates to load world when joining online sessions.
}

void UFindAWayOutWorldManager::Deinitialize()
{
	Super::Deinitialize();
}

TSubclassOf<UPulseSaveModel> UFindAWayOutWorldManager::GetSaveModelClass_Implementation(UPulseSaveManager* SaveManager) const
{
	return UFindAWayOutWorldManagerSaveModel::StaticClass();
}

void UFindAWayOutWorldManager::NotifyWorldChanged(AFindAWayOutGameMode* GameMode)
{
	AFindAWayOutWorldSettings* worldSettings = CastChecked<AFindAWayOutWorldSettings>(GameMode->GetWorldSettings());
	UFindAWayOutWorldDefinition* worldDefinition = worldSettings->WorldDefinition.LoadSynchronous();
	
	check(worldDefinition);
	CurrentWorldTag = worldDefinition->WorldTag;
	
	UFindAWayOutSaveSubsystem& saveSubsystem = UFindAWayOutSaveSubsystem::Get(this);
	
	if (saveSubsystem.WorldSaves.Contains(CurrentWorldTag))
	{
		saveSubsystem.SetWorldSaveGame(saveSubsystem.WorldSaves.FindRef(CurrentWorldTag));
		saveSubsystem.LoadCurrentWorldSave();
	}
	else
	{
		// Add a default save game for the new world.
		
		FPulseSaveGame& saveGame = saveSubsystem.WorldSaves.Add(CurrentWorldTag); // This would also work without a reference to the newly added save game since a FPulseSaveGame is basically just a TSharedPtr.
		
		saveSubsystem.SetWorldSaveGame(saveGame);
		OnNewWorldSaveGameCreated(saveGame);
	}
	
	OnWorldChanged.Broadcast(this);
}

void UFindAWayOutWorldManager::OnNewWorldSaveGameCreated(FPulseSaveGame NewSaveGame)
{
}

void UFindAWayOutWorldManager::OnLoad_Implementation(UPulseSaveManager* SaveManager, UPulseSaveModel* SaveModel)
{
	UFindAWayOutWorldManagerSaveModel* managerModel = CastChecked<UFindAWayOutWorldManagerSaveModel>(SaveModel);
	const FGameplayTag worldToLoad = managerModel->SavedWorldTag;
	
	if (worldToLoad != CurrentWorldTag)
		LoadWorldByTag(worldToLoad);
}

EPulseSaveChoice UFindAWayOutWorldManager::IsReadyToLoad_Implementation(UPulseSaveManager* SaveManager, const UPulseSaveModel* SaveModel) const
{
	const UFindAWayOutWorldManagerSaveModel* model = Cast<UFindAWayOutWorldManagerSaveModel>(SaveModel);
	return Pulse::Save::BoolToChoice(CurrentWorldTag == model->SavedWorldTag);
}

void UFindAWayOutWorldManager::OnWorldSaveComplete_ChangeWorld(UPulseSaveManager* SaveManager, bool bSuccess)
{	
	if (!bSuccess)
	{
		UE_LOG(LogFindAWayOut, Error, TEXT("UFindAWayOutWorldManager::OnWorldSaveComplete_ChangeWorld - Failed to save world before changing world to %s"), *RequestedWorldTag.GetTagName().ToString());
		RequestedWorldTag = FGameplayTag::EmptyTag;
		
		return;
	}
	
	LoadWorldByTag(RequestedWorldTag);
	RequestedWorldTag = FGameplayTag::EmptyTag;
}

bool UFindAWayOutWorldManager::LoadWorldByTag(const FGameplayTag& WorldTag)
{
	// const bool bSavingOrLoading = UFindAWayOutSaveSubsystem::Get(this).IsSavingOrLoading();
	// if (!ensureMsgf(!bSavingOrLoading, TEXT("UFindAWayOutWorldManager::LoadWorldByTag - Cannot load world with tag %s while saving or loading is in progress."), *WorldTag.GetTagName().ToString()))
	// 	return false;
	
	const UFindAWayOutSettings* settings = GetDefault<UFindAWayOutSettings>();
	UFindAWayOutWorldList& worldList = *settings->WorldList.LoadSynchronous();
	
	const FFindAWayOutWorldEntry* worldEntry = worldList.FindEntry(WorldTag);
	if (!worldEntry)
	{
		UE_LOG(LogFindAWayOut, Error, TEXT("UFindAWayOutWorldManager::LoadWorldByTag - Failed to find world entry for tag %s"), *WorldTag.GetTagName().ToString());
		return false;
	}
	
	const FName levelName = FName(*FPackageName::ObjectPathToPackageName(worldEntry->World.ToString()));
	UWorld* world = GetWorld();
	
	TStringBuilder<256> urlBuilder;
	
	urlBuilder.Append(levelName.ToString());
	urlBuilder.Append(TEXT("?listen"));
	
	return world->ServerTravel(urlBuilder.ToString(), TRAVEL_Absolute);
}

bool UFindAWayOutWorldManager::RequestWorldChange(FGameplayTag WorldTag, bool bSaveCurrentWorld)
{
	if (!WorldTag.IsValid() || WorldTag == CurrentWorldTag || RequestedWorldTag.IsValid())
		return false;
	
	if (bSaveCurrentWorld)
	{
		UFindAWayOutSaveSubsystem& saveSubsystem = UFindAWayOutSaveSubsystem::Get(this);
	
		RequestedWorldTag = WorldTag;
		if (!saveSubsystem.SaveCurrentWorld(FPulseSaveManagerProcessedSignature::FDelegate::CreateUObject(this, &UFindAWayOutWorldManager::OnWorldSaveComplete_ChangeWorld)))
		{
			RequestedWorldTag = FGameplayTag::EmptyTag;
			return false;
		}
	
		return true;
	}
	
	return LoadWorldByTag(WorldTag);
}
