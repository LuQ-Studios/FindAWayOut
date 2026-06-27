// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutSaveSubsystem.h"

#include "FindAWayOut.h"
#include "FindAWayOutSettings.h"
#include "FindAWayOutWorldManager.h"
#include "LoadingScreenManager.h"
#include "PlatformFeatures.h"
#include "PulseSaveUtils.h"
#include "SaveGameSystem.h"
#include "Collectors/FindAWayOutGameSaveCollector.h"
#include "Collectors/FindAWayOutWorldSaveCollector.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Models/FindAWayOutSaveSubsystemModel.h"

const FName UFindAWayOutSaveSubsystem::SaveID = TEXT("FindAWayOutSaveSubsystem");

UFindAWayOutSaveSubsystem& UFindAWayOutSaveSubsystem::Get(const UObject* WorldContextObject)
{
	UWorld* world = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	UFindAWayOutSaveSubsystem* saveSubsystem = world->GetGameInstance()->GetSubsystem<UFindAWayOutSaveSubsystem>();
	
	check(saveSubsystem);
	return *saveSubsystem;
}

void UFindAWayOutSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GameSaveManager = NewObject<UPulseSaveManager>(this);
	GameSaveCollector = CastChecked<UFindAWayOutGameSaveCollector>(GameSaveManager->AddSaveObjectCollectorByClass(UFindAWayOutGameSaveCollector::StaticClass()));
	
	GameSaveCollector->RegisterObject(this, FPulseSaveRegisterCollectorParams().LoadOnRegister(false));
	
	WorldSaveManager = NewObject<UPulseSaveManager>(this);
	WorldSaveCollector = CastChecked<UFindAWayOutWorldSaveCollector>(WorldSaveManager->AddSaveObjectCollectorByClass(UFindAWayOutWorldSaveCollector::StaticClass()));
	
	ULoadingScreenManager* loadingScreenManager = Collection.InitializeDependency<ULoadingScreenManager>();
	loadingScreenManager->RegisterLoadingProcessor(this);
}

void UFindAWayOutSaveSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

FName UFindAWayOutSaveSubsystem::GetUniqueSaveIdentifier_Implementation() const
{
	return SaveID;
}

TSubclassOf<UPulseSaveModel> UFindAWayOutSaveSubsystem::GetSaveModelClass_Implementation(UPulseSaveManager* SaveManager) const
{
	return UFindAWayOutSaveSubsystemModel::StaticClass();
}

bool UFindAWayOutSaveSubsystem::ShouldShowLoadingScreen(FString& OutReason) const
{
	if (GameSaveManager->GetProcessState() == EPulseSaveManagerProcessType::Loading)
	{
		OutReason = TEXT("Loading Game Save");
		return true;
	}
	
	if (WorldSaveManager->GetProcessState() == EPulseSaveManagerProcessType::Loading)
	{
		OutReason = TEXT("Loading World Save");
		return true;
	}
	
	return false;
}

void UFindAWayOutSaveSubsystem::SetWorldSaveGame(const FPulseWeakSaveGame& NewWorldSave)
{
	if (!NewWorldSave.IsValid())
	{
		CurrentWorldSave.Reset();
		WorldSaveManager->SetSaveGame(NewWorldSave);
		
		return;
	}
	
	CurrentWorldSave = NewWorldSave.ToStrong();
	WorldSaveManager->SetSaveGame(NewWorldSave);
}

void UFindAWayOutSaveSubsystem::LoadCurrentWorldSave()
{
	check(CurrentWorldSave.IsValid());
	const bool bLoadStarted = WorldSaveManager->Load();
	
	UE_CLOG(!bLoadStarted, LogFindAWayOut, Fatal, TEXT("UFindAWayOutSaveSubsystem::LoadCurrentWorldSave - Failed to start loading world save."));
}

bool UFindAWayOutSaveSubsystem::SaveCurrentWorld(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback)
{
	return WorldSaveManager->Save(OnCompleteCallback);
}

bool UFindAWayOutSaveSubsystem::LoadCurrentWorld(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback)
{
	return WorldSaveManager->Load(OnCompleteCallback);
}

void UFindAWayOutSaveSubsystem::InternalSetGameSave(const FPulseSaveGame& NewSaveGame, const FString& SlotName, bool bLoadSaveGame, const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback)
{
	if (GameSaveManager->GetProcessState() != EPulseSaveManagerProcessType::Idle)
		GameSaveManager->AbortProcess();
	
	CurrentSaveSlot = SlotName;
	CurrentGameSave = NewSaveGame;
	
	SetWorldSaveGame(FPulseWeakSaveGame());
	
	WorldSaves.Reset();
	GameSaveManager->SetSaveGame(CurrentGameSave);
	
	if (bLoadSaveGame)
	{
		const bool bLoaded = LoadCurrentSaveGame(OnCompleteCallback);
		checkf(bLoaded, TEXT("UFindAWayOutSaveSubsystem::InternalSetGameSave - Failed to load game save from slot %s."), *SlotName);
	}
}

void UFindAWayOutSaveSubsystem::OnNewGameCreated()
{
	WorldSaves.Reset();
	CurrentWorldSave.Reset();
}

bool UFindAWayOutSaveSubsystem::SaveWorld()
{
	return SaveCurrentWorld();
}

bool UFindAWayOutSaveSubsystem::LoadWorld()
{
	return LoadCurrentWorld();
}

bool UFindAWayOutSaveSubsystem::IsSavingOrLoading() const
{
	return IsGameSavingOrLoading() || IsWorldSavingOrLoading();
}

bool UFindAWayOutSaveSubsystem::IsGameSavingOrLoading() const
{
	return GameSaveManager->GetProcessState() != EPulseSaveManagerProcessType::Idle;
}

bool UFindAWayOutSaveSubsystem::IsWorldSavingOrLoading() const
{
	return WorldSaveManager->GetProcessState() != EPulseSaveManagerProcessType::Idle;
}

bool UFindAWayOutSaveSubsystem::CreateNewGame(const FString& SlotName)
{
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogFindAWayOut, Warning, TEXT("UFindAWayOutSaveSubsystem::CreateNewGame - Slot name is empty. Cannot create new game."));
		return false;
	}
	
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		UE_LOG(LogFindAWayOut, Warning, TEXT("UFindAWayOutSaveSubsystem::CreateNewGame - Save slot '%s' already exists. Cannot create new game."), *SlotName);
		return false;
	}
	
	InternalSetGameSave(FPulseSaveGame::New(), SlotName, false);
	OnNewGameCreated();
	
	const bool bWorldChangeRequested = UFindAWayOutWorldManager::Get(this).RequestWorldChange(UFindAWayOutSettings::Get().FirstGameWorldTag, false);
	UE_CLOG(!bWorldChangeRequested, LogFindAWayOut, Fatal, TEXT("UFindAWayOutSaveSubsystem::CreateNewGame - Failed to request world change to first game world with tag %s."), *UFindAWayOutSettings::Get().FirstGameWorldTag.GetTagName().ToString());
	
	return true;
}

bool UFindAWayOutSaveSubsystem::SaveGame()
{
	return SaveCurrentGame(FPulseSaveManagerProcessedSignature::FDelegate::CreateLambda([this](UPulseSaveManager* Manager, bool bSuccess)
	{
		if (!bSuccess)
		{
			UE_LOG(LogFindAWayOut, Error, TEXT("UFindAWayOutSaveSubsystem::SaveCurrentGame - Failed to save current game."));
			return;
		}
		
		// Save game to slot
		
		if (!UPulseSaveUtils::SaveGameToSlot(CurrentGameSave.Pin(), CurrentSaveSlot, 0))
		{
			UE_LOG(LogFindAWayOut, Error, TEXT("UFindAWayOutSaveSubsystem::SaveCurrentGame - Failed to save game to slot '%s'."), *CurrentSaveSlot);
			return;
		}
	}));
}

bool UFindAWayOutSaveSubsystem::SaveCurrentGame(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback)
{
	if (!CurrentGameSave.IsValid() || CurrentSaveSlot.IsEmpty())
	{
		UE_LOG(LogFindAWayOut, Warning, TEXT("UFindAWayOutSaveSubsystem::SaveCurrentGame - No valid current game save or save slot. Cannot save game."));
		return false;
	}
	
	return GameSaveManager->Save(OnCompleteCallback);
}

bool UFindAWayOutSaveSubsystem::LoadGame(const FString& SlotName)
{
	FPulseStrongSaveGame loadedGame = UPulseSaveUtils::LoadGameFromSlot(SlotName, 0);
	if (!loadedGame.IsValid())
	{
		UE_LOG(LogFindAWayOut, Warning, TEXT("UFindAWayOutSaveSubsystem::LoadGame - No valid save game found in slot '%s'."), *SlotName);
		return false;
	}
	
	InternalSetGameSave(loadedGame.Pin(), SlotName, true);
	return true;
}

bool UFindAWayOutSaveSubsystem::LoadCurrentSaveGame(const FPulseSaveManagerProcessedSignature::FDelegate& OnCompleteCallback)
{
	return GameSaveManager->Load(OnCompleteCallback);
}

bool UFindAWayOutSaveSubsystem::GetExistingSaveSlots(TArray<FString>& OutSaveSlots) const
{
	return IPlatformFeaturesModule::Get().GetSaveGameSystem()->GetSaveGameNames(OutSaveSlots, 0);
}
