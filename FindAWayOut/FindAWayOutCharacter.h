// Copyright (c) 2026, LuQ Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InputActionValue.h"
#include "NativeGameplayTags.h"
#include "Abilities/FindAWayOutAbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/FindAWayOutSaveInterface.h"
#include "FindAWayOutCharacter.generated.h"

class UFindAWayOutCharacterInputData;

UCLASS(Abstract)
class FINDAWAYOUT_API AFindAWayOutCharacter : public ACharacter, public IAbilitySystemInterface, public IFindAWayOutSaveInterface
{
	GENERATED_BODY()

public:

	AFindAWayOutCharacter();

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
public:
	
	virtual FName GetUniqueSaveIdentifier_Implementation() const override;
	virtual bool ShouldExcludeFromWorldSave_Implementation() const override { return true; }
	
protected:
	
	/** Move input action callback */
	void MoveAction(const FInputActionValue& Value);
	
	/** Look input action callback */
	void LookAction(const FInputActionValue& Value);
	
	/** Interact action callback */
	void InteractAction(const FInputActionValue& Value);
	
public:
	
	/** Input data for this character. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UFindAWayOutCharacterInputData> CharacterInputData;
	
public:
	
	/** Ability system component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UFindAWayOutAbilitySystemComponent> AbilitySystemComponent;
	
	/** Initial abilities to grant to the player.  */
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FFindAWayOutAbility> InitialAbilities;
	
public:
	
	/** Identifier for this character */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character")
	FGameplayTag CharacterIdentifier;
};

namespace FindAWayOut::Character
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_01);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_02);
}