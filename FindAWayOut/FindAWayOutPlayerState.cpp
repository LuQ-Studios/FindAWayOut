// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutPlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "FindAWayOut.h"
#include "GameplayAbilitySpec.h"
#include "PulseUtils.h"
#include "Components/PulseQuestManager.h"
#include "Save/FindAWayOutSaveSubsystem.h"
#include "Save/Collectors/FindAWayOutGameSaveCollector.h"
#include "Save/Models/FindAWayOutPlayerStateSaveModel.h"

AFindAWayOutPlayerState::AFindAWayOutPlayerState()
{
	bReplicateUsingRegisteredSubObjectList = true; // This is the recommended way for quest manager replication
	
	QuestManager = CreateDefaultSubobject<UPulseQuestManager>("QuestManager");
	QuestManager->SetIsReplicated(true); // Set replicated since quest manager will only run on server.
	QuestManager->bAutoManageAvatarActor = true;
}

void AFindAWayOutPlayerState::OnDeactivated()
{
	if (HasAuthority())
	{
		UFindAWayOutSaveSubsystem& saveSubsystem = UFindAWayOutSaveSubsystem::Get(this);
		
		saveSubsystem.GameSaveCollector->UnregisterObject(this);
		saveSubsystem.GameSaveManager->SaveSingleObject(this, FPulseSaveManagerProcessedSingleDelegate::CreateLambda([this](UPulseSaveManager*, UObject*, bool bSuccess)
		{
			if (!bSuccess)
				UE_LOG(LogFindAWayOut, Error, TEXT("Failed to save player state on deactivation"));
		
			Destroy();
		}));
	}
}

FName AFindAWayOutPlayerState::GetUniqueSaveIdentifier_Implementation() const
{
	ULocalPlayer* player = Pulse::Utils::GetLocalPlayer(GetOwningController());
	check(player);
	
	return FName(FString::FromInt(player->GetLocalPlayerIndex()));
	
	//return FName(FString::FromInt(GetPlayerId()));
	//return FName(GetUniqueId()->ToString());
}

TSubclassOf<UPulseSaveModel> AFindAWayOutPlayerState::GetSaveModelClass_Implementation(UPulseSaveManager* SaveManager) const
{
	return UFindAWayOutPlayerStateSaveModel::StaticClass();
}

void AFindAWayOutPlayerState::OnLoadFailed_Implementation(UPulseSaveManager* SaveManager, const UPulseSaveModel* SaveModel, EPulseSaveLoadFailureType FailureType)
{
	if (FailureType == EPulseSaveLoadFailureType::ObjectNotFound)
		InitializeNewPlayer();
}

void AFindAWayOutPlayerState::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
		UFindAWayOutSaveSubsystem::Get(this).GameSaveCollector->RegisterObject(this);
}

void AFindAWayOutPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);
	
	if (HasAuthority())
		UFindAWayOutSaveSubsystem::Get(this).GameSaveCollector->UnregisterObject(this);
	
	// Save quest manager to new player state
	
	// AFindAWayOutPlayerState* newPlayerState = CastChecked<AFindAWayOutPlayerState>(PlayerState);
	//
	// const bool bSavedQuestManager = QuestManager->SaveQuestManager(newPlayerState->QuestManagerSave, false);
	// UE_CLOG(!bSavedQuestManager, LogFindAWayOut, Fatal, TEXT("Failed to save quest manager for player state %s. Quest manager must be successfully saved in order to properly transfer quest data to the new player state."), *GetName());
}

void AFindAWayOutPlayerState::InitializeNewPlayer()
{
}

UAbilitySystemComponent* AFindAWayOutPlayerState::GetAbilitySystemComponent() const
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
}
