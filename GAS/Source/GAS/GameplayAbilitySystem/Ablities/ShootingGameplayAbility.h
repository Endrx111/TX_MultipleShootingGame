// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ShootingGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UShootingGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	bool bShouldShowInAbilitiesBar = false;
};
