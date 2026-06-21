// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingAbilitySystemComponent.h"

#include "Characters/Shooting.h"


// Sets default values for this component's properties
UShootingAbilitySystemComponent::UShootingAbilitySystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShootingAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UShootingAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	AShooting* Character = Cast<AShooting>(GetOwner());
	
	if (!Character)return;
	
	bool bAbilitiesChanged =false;
	
	if (LastActiveAbility.Num()!=ActivatableAbilities.Items.Num())//防止重复能力。添加或者删除了新能力，能力数量更新触发
	{
		bAbilitiesChanged =true;
	}
	else
	{
		for (int32 i=0;i<LastActiveAbility.Num();i++)
		{
			if (LastActiveAbility[i].Ability != ActivatableAbilities.Items[i].Ability)
			{
				
				bAbilitiesChanged =true;
				break;
			}
			
		}
		
	}
	if (bAbilitiesChanged)
	{
		Character->SendAbilitiesChangeEvent();
		LastActiveAbility = ActivatableAbilities.Items;
	}
}


// Called every frame
void UShootingAbilitySystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                    FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

