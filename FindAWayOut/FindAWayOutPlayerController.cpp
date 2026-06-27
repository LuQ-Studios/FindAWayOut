// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutPlayerController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "EnhancedInputComponent.h"
#include "FindAWayOutInputData.h"
#include "FindAWayOutPlayerState.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "UObject/ConstructorHelpers.h"

AFindAWayOutPlayerController::AFindAWayOutPlayerController()
{
	// ConstructorHelpers::FObjectFinder<UFindAWayOutControllerInputData> inputData(TEXT("/Game/Content/Input/DA_ControllerInput.DA_ControllerInput"));
	// if (inputData.Succeeded())
	// 	ControllerInputData = inputData.Object;
}

void AFindAWayOutPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		UFindAWayOutControllerInputData* data = ControllerInputData.LoadSynchronous();
		if (ensureMsgf(data, TEXT("Controller input data is not set on player controller %s Please set it to properly bind input contexts."), *GetName()))
		{
			const bool bAppliedContexts = data->ApplyInputContexts(GetLocalPlayer());
			ensureMsgf(bAppliedContexts, TEXT("Failed to apply one or more input contexts for player controller %s. Please check that all input contexts set on the controller input data are valid and load properly."), *GetName());
		}
	}
}

void AFindAWayOutPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsLocalController())
	{
		if (UFindAWayOutControllerInputData* data = ControllerInputData.LoadSynchronous())
			data->RemoveInputContexts(GetLocalPlayer());
	}
	
	Super::EndPlay(EndPlayReason);
}

void AFindAWayOutPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UFindAWayOutControllerInputData* data = ControllerInputData.LoadSynchronous();
	if (ensureMsgf(data, TEXT("Controller input data is not set on player controller %s Please set it to properly bind ability input."), *GetName()))
	{
		UEnhancedInputComponent* component = CastChecked<UEnhancedInputComponent>(InputComponent);
		for (const TPair<TObjectPtr<UInputAction>, EFindAWayOutAbilityInputID>& pair : ControllerInputData->AbilityInputActions)
		{
			if (pair.Key)
			{
				component->BindAction(pair.Key, ETriggerEvent::Started, this, &AFindAWayOutPlayerController::AbilityInputPressed, pair.Value);
				component->BindAction(pair.Key, ETriggerEvent::Completed, this, &AFindAWayOutPlayerController::AbilityInputReleased, pair.Value);
			}
		}
		
		component->BindAction(data->PauseMenuAction, ETriggerEvent::Started, this, &AFindAWayOutPlayerController::TogglePauseMenu);
	}
}

UAbilitySystemComponent* AFindAWayOutPlayerController::GetAbilitySystemComponent() const
{
	return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
}

APlayerState* AFindAWayOutPlayerController::GetQuestPlayerState_Implementation() const
{
	return PlayerState;
}

void AFindAWayOutPlayerController::OnAddedToQuestManager_Implementation(APulseSharedQuestHost* NewManager)
{
	check(NewManager);
	if (ensureMsgf(!SharedQuestHost, TEXT("Player controller %s is already part of a shared quest host."), *GetName()))
		SharedQuestHost = NewManager;
}

void AFindAWayOutPlayerController::OnRemovedFromQuestManager_Implementation(APulseSharedQuestHost* OldManager)
{
	check(OldManager);
	if (ensureMsgf(SharedQuestHost == OldManager, TEXT("Player controller %s is being removed from a shared quest host it is not part of."), *GetName()))
		SharedQuestHost = nullptr;
}

void AFindAWayOutPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
}

void AFindAWayOutPlayerController::TogglePauseMenu()
{
}

void AFindAWayOutPlayerController::OpenPauseMenu()
{
}

void AFindAWayOutPlayerController::ClosePauseMenu()
{
}

void AFindAWayOutPlayerController::AbilityInputPressed(EFindAWayOutAbilityInputID InputID)
{
	GetAbilitySystemComponent()->AbilityLocalInputPressed(static_cast<int32>(InputID));
}

void AFindAWayOutPlayerController::AbilityInputReleased(EFindAWayOutAbilityInputID InputID)
{
	GetAbilitySystemComponent()->AbilityLocalInputReleased(static_cast<int32>(InputID));
}