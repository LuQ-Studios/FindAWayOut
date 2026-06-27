// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "FindAWayOutAbilityTypes.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/FindAWayOutSaveInterface.h"
#include "Interfaces/PulseSharedQuestPlayerInterface.h"
#include "Quest/Save/PulseQuestSaveTypes.h"
#include "FindAWayOutPlayerController.generated.h"

class UFindAWayOutControllerInputData;
class UPulseGameplayAbility;
class UPulseQuestManager;

/**
 * 
 */
UCLASS(Abstract)
class FINDAWAYOUT_API AFindAWayOutPlayerController : public APlayerController, public IAbilitySystemInterface, public IPulseSharedQuestPlayerInterface
{
	GENERATED_BODY()	
public:
	
	AFindAWayOutPlayerController();
	
public:
	
	// ~Begin APlayerController
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;
	// ~End APlayerController
	
	// ~Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~End IAbilitySystemInterface
	
	// ~Begin IPulseSharedQuestPlayerInterface
	virtual APlayerState* GetQuestPlayerState_Implementation() const override;
	virtual void OnAddedToQuestManager_Implementation(APulseSharedQuestHost* NewManager) override;
	virtual void OnRemovedFromQuestManager_Implementation(APulseSharedQuestHost* OldManager) override;
	virtual void SetPawn(APawn* InPawn) override;
	// ~End IPulseSharedQuestPlayerInterface
	
public:
	
	/** Shared host this player controller is part of. */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Quest")
	TObjectPtr<APulseSharedQuestHost> SharedQuestHost;
	
public:
	
	/** Input data for this controller */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UFindAWayOutControllerInputData> ControllerInputData;
	
protected:
	
	/** Pause menu widget instance */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "UI")
	TObjectPtr<UUserWidget> PauseMenuWidget;
	
public:
	
	void TogglePauseMenu();
	void OpenPauseMenu();
	void ClosePauseMenu();
	
protected:
	
	void AbilityInputPressed(EFindAWayOutAbilityInputID InputID);
	void AbilityInputReleased(EFindAWayOutAbilityInputID InputID);
};
