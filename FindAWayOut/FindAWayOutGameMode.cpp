// Copyright (c) 2026, LuQ Studios. All Rights Reserved.


#include "FindAWayOutGameMode.h"

#include "FindAWayOutCharacter.h"
#include "FindAWayOutPlayerController.h"
#include "FindAWayOutWorldManager.h"
#include "Quest/PulseSharedQuestHost.h"

void AFindAWayOutGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	{
		FActorSpawnParameters spawnParams;
	
		spawnParams.Instigator = GetInstigator();
		spawnParams.ObjectFlags |= RF_Transient;
	
		SharedQuestHost = GetWorld()->SpawnActor<APulseSharedQuestHost>(SharedQuestHostClass, spawnParams);
		check(SharedQuestHost);
	}
	
	// Spawn characters
	
	{
		FActorSpawnParameters spawnParams;
		
		spawnParams.ObjectFlags |= RF_Transient;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		// TODO: Spawn location
		
		AActor* playerStart = ChoosePlayerStart(nullptr);
		check(playerStart);
		
		FirstCharacter = GetWorld()->SpawnActor<AFindAWayOutCharacter>(FirstCharacterClass, playerStart->GetActorLocation(), playerStart->GetActorRotation(), spawnParams);
		check(FirstCharacter);
		
		SecondCharacter = GetWorld()->SpawnActor<AFindAWayOutCharacter>(SecondCharacterClass, playerStart->GetActorLocation(), playerStart->GetActorRotation(), spawnParams);
		check(SecondCharacter);
	}
	
	UFindAWayOutWorldManager::Get(this).NotifyWorldChanged(this);
}

void AFindAWayOutGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//SharedQuestHost->AddPlayer(NewPlayer);
}

void AFindAWayOutGameMode::Logout(AController* Exiting)
{
	if (AFindAWayOutPlayerController* controller = Cast<AFindAWayOutPlayerController>(Exiting))
		SharedQuestHost->RemovePlayer(controller);
		
	Super::Logout(Exiting);
}

void AFindAWayOutGameMode::RestartPlayer(AController* NewPlayer)
{
	if (!FirstCharacter->IsPlayerControlled())
		NewPlayer->Possess(FirstCharacter);
	else if (!SecondCharacter->IsPlayerControlled())
		NewPlayer->Possess(SecondCharacter);
}
