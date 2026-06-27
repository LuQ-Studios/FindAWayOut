// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataAsset.h"
#include "FindAWayOutWorldList.generated.h"

class UFindAWayOutWorldDefinition;

USTRUCT(BlueprintType)
struct FFindAWayOutWorldEntry
{
	GENERATED_BODY()
public:
	
	/** Definition of the world */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	TSoftObjectPtr<UFindAWayOutWorldDefinition> WorldDefinition;
	
	/** World reference */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	TSoftObjectPtr<UWorld> World;
};

/**
 * 
 */
UCLASS()
class FINDAWAYOUT_API UFindAWayOutWorldList : public UDataAsset
{
	GENERATED_BODY()
public:
	
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	
public:
	
	const FFindAWayOutWorldEntry* FindEntry(const FGameplayTag& InWorldTag) const;
	
public:
	
	/** World list */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World")
	TArray<FFindAWayOutWorldEntry> WorldList;
};
