// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UBasicAttributeSet::UBasicAttributeSet()
{
	Health=100.0f;
	MaxHealth=100.0f;	
	Stamina=100.0f;
	MaxStamina=100.0f;
	
	
}

void UBasicAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet,Stamina,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBasicAttributeSet,MaxStamina,COND_None,REPNOTIFY_Always);
}

void UBasicAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);//Super-Parent 当有代码或蓝图手动修改属性值时被调用
	if (Attribute==GetHealthAttribute())
	{
		NewValue=FMath::Clamp(NewValue,0.f,GetMaxHealth());
	}
	else if (Attribute==GetStaminaAttribute())
	{
		
		NewValue=FMath::Clamp(NewValue,0.f,GetMaxStamina());
	}
	
}

void UBasicAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		//SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
		SetHealth(GetHealth());//触发上面的PreAttributeChange
		
		if (Data.EffectSpec.Def->GetAssetTags().HasTag(FGameplayTag::RequestGameplayTag("Effect.HitReaction")))
		{
			
			FGameplayTagContainer HitReactionTagContainer;
			HitReactionTagContainer.AddTag((FGameplayTag::RequestGameplayTag(FName("GA.HitReaction"))));
			GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(HitReactionTagContainer);
			
		
		}
		
	}
	
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		//SetStamina(FMath::Clamp(GetStamina(),0.f,GetMaxStamina()));
		SetStamina(GetStamina());
	}
}


void UBasicAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute==GetHealthAttribute()&&  NewValue<=0.f)
	{
		
			
		FGameplayTagContainer DeathTagContainer;
		DeathTagContainer.AddTag((FGameplayTag::RequestGameplayTag(FName("GA.Death"))));
		GetOwningAbilitySystemComponent()->TryActivateAbilitiesByTag(DeathTagContainer);
		
		
	}
}


