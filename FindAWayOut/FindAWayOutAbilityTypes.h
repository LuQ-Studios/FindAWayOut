// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FindAWayOutAbilityTypes.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class EFindAWayOutAbilityInputID : uint8
{
	None = 0,
	Confirm,
	Cancel
};

USTRUCT(BlueprintType)
struct FFindAWayOutAbility
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSoftClassPtr<UGameplayAbility> Ability;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	int32 Level = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	EFindAWayOutAbilityInputID InputID = EFindAWayOutAbilityInputID::None;
};