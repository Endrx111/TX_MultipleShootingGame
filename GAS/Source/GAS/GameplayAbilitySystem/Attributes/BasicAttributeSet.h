// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BasicAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UBasicAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
	
public:
	
	UBasicAttributeSet();
	UPROPERTY( BlueprintReadOnly, Category = "Attributes",ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet,Health);//class and the var wewantto change
	
	UPROPERTY( BlueprintReadOnly, Category = "Attributes",ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet,MaxHealth);
	//Stamina Attributes
	UPROPERTY( BlueprintReadOnly, Category = "Attributes",ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet,Stamina);
	
	UPROPERTY( BlueprintReadOnly, Category = "Attributes",ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS_BASIC(UBasicAttributeSet,MaxStamina);
	
public:
	//multiplayer Replicate notify
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const
	{
		
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet,Health,OldHealth);
	}
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet,MaxHealth,OldMaxHealth);
	}
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet,Stamina,OldStamina);
	}
	
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UBasicAttributeSet,MaxStamina,OldMaxStamina);
	}
	
	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;//IF SYNC to (owner)Client?
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute,float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;//游戏性效果应用后被调用
	
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
};
