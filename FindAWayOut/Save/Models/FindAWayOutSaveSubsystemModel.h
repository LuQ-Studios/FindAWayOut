// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "PulseSaveManager.h"
#include "PulseSaveModel.h"
#include "FindAWayOutSaveSubsystemModel.generated.h"

enum class EFindAWayOutSaveSubsystemModelWorldSaveResult : uint8
{
	InProgress,
	
	Success,
	
	Failure
};

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutSaveSubsystemModel : public UPulseSaveModel
{
	GENERATED_BODY()
public:
	
	UFindAWayOutSaveSubsystemModel();
	
public:
	
	virtual bool OnPreSave() override;
	virtual bool OnPostSave() override;
	virtual bool OnPreLoad() override;
	virtual bool OnPostLoad() override;
	virtual bool IsReadyToSave() const override;
	virtual bool IsReadyToLoad() const override;
	virtual bool ShouldBlockProcess() const override { return GetOuterUPulseSaveManager()->GetProcessState() == EPulseSaveManagerProcessType::Loading; }
	
private:
	
	EFindAWayOutSaveSubsystemModelWorldSaveResult WorldSaveResult = EFindAWayOutSaveSubsystemModelWorldSaveResult::InProgress;
	void WorldSaveComplete(UPulseSaveManager* SaveManager, bool bSuccess);
	
public:
	
	/** World saves mapped to their world tag */
	UPROPERTY(SaveGame)
	TMap<FGameplayTag, FPulseSaveGame> WorldSaves;
};
