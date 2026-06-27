// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "FindAWayOutAbilityTypes.h"
#include "Abilities/FindAWayOutAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/FindAWayOutSaveInterface.h"
#include "Quest/Save/PulseQuestSaveTypes.h"
#include "FindAWayOutPlayerState.generated.h"

class UPulseQuestManager;



/**
 * 
 */
UCLASS(Abstract)
class FINDAWAYOUT_API AFindAWayOutPlayerState : public APlayerState, public IFindAWayOutSaveInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()	
public:
	
	AFindAWayOutPlayerState();
	
public:
	
	virtual void OnDeactivated() override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	virtual FName GetUniqueSaveIdentifier_Implementation() const override;
	virtual TSubclassOf<UPulseSaveModel> GetSaveModelClass_Implementation(UPulseSaveManager* SaveManager) const override;
	virtual void OnLoadFailed_Implementation(UPulseSaveManager* SaveManager, const UPulseSaveModel* SaveModel, EPulseSaveLoadFailureType FailureType) override;
	virtual bool ShouldExcludeFromWorldSave_Implementation() const override { return true; }
	
protected:
	
	virtual void BeginPlay() override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	
protected:
	
	void InitializeNewPlayer();

public:
	
	/** Quest manager */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TObjectPtr<UPulseQuestManager> QuestManager;
};
