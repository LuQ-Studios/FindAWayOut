// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "Interfaces/FindAWayOutSaveInterface.h"
#include "FindAWayOutGameMode.generated.h"

class AFindAWayOutCharacter;
class AFindAWayOutPlayerController;
class APulseSharedQuestHost;

/**
 * 
 */
UCLASS(Abstract)
class FINDAWAYOUT_API AFindAWayOutGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	
	// ~Begin AGameModeBase
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	// ~End AGameModeBase
	
public:
	
	/** Shared quest host class */
	UPROPERTY(EditAnywhere, Category = "Quest")
	TSubclassOf<APulseSharedQuestHost> SharedQuestHostClass;
	
	/** Shared quest host */
	UPROPERTY(BlueprintReadOnly, Category = "Quest")
	TObjectPtr<APulseSharedQuestHost> SharedQuestHost;
	
public:
	
	/** First character class */
	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<AFindAWayOutCharacter> FirstCharacterClass;
	
	/** Second character class */
	UPROPERTY(EditAnywhere, Category = "Player")
	TSubclassOf<AFindAWayOutCharacter> SecondCharacterClass;
	
	/** First character instance */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Player")
	TObjectPtr<AFindAWayOutCharacter> FirstCharacter;
	
	/** Second character instance */
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Player")
	TObjectPtr<AFindAWayOutCharacter> SecondCharacter;
};
