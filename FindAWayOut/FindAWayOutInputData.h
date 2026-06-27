// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "FindAWayOutAbilityTypes.h"
#include "InputMappingContext.h"
#include "Engine/DataAsset.h"
#include "FindAWayOutInputData.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FFindAWayOutInputContext
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> MappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	int32 Priority = 0;
};

UCLASS(Abstract)
class FINDAWAYOUT_API UFindAWayOutInputDataBase : public UDataAsset
{
	GENERATED_BODY()
public:
	
	bool ApplyInputContexts(ULocalPlayer* InPlayer) const;
	bool RemoveInputContexts(ULocalPlayer* InPlayer) const;
	
public:
	
	/** Pause menu action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> PauseMenuAction;
	
	/** Input contexts to apply when this controller is active. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TArray<FFindAWayOutInputContext> InputContexts;
};

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutControllerInputData : public UFindAWayOutInputDataBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability")
	TMap<TObjectPtr<UInputAction>, EFindAWayOutAbilityInputID> AbilityInputActions;
	
	
};

UCLASS()
class FINDAWAYOUT_API UFindAWayOutCharacterInputData : public UFindAWayOutInputDataBase
{
	GENERATED_BODY()
public:
	
	/** Move action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	/** Look action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	/** Interaction action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
};