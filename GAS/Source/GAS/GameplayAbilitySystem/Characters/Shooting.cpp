// Fill out your copyright notice in the Description page of Project Settings.


#include "Shooting.h"
#include "GAS/GameplayAbilitySystem/ShootingAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/GameplayAbilitySystem/Attributes/BasicAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/GameplayAbilitySystem/ShootingAbilitySystemComponent.h"
#include "Math/UnitConversion.h"
// Sets default values
AShooting::AShooting()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//add ASC
	AbilitySystemComponent = CreateDefaultSubobject<UShootingAbilitySystemComponent>(TEXT("AbilitySystemComponent"));// add component
	AbilitySystemComponent->SetIsReplicated(true);//multipleGame
	AbilitySystemComponent->SetReplicationMode(AscReplicationMode);//replicationMode
	
	
	GetCapsuleComponent()->InitCapsuleSize(35.0f,90.0f);
	
	//Dont Rotate when the controller rotate
	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
	
	GetCharacterMovement()->bOrientRotationToMovement=true;//character rotate to movement direction
	GetCharacterMovement()->RotationRate=FRotator(0.0f,500.0f,0.0f);
	
	GetCharacterMovement()->JumpZVelocity=500.0f;
	GetCharacterMovement()->AirControl=0.35f;
	GetCharacterMovement()->MaxWalkSpeed=600.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed=20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking=2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling=1500.0f;
//add Attribute
BasicAttributeSet = CreateDefaultSubobject<UBasicAttributeSet>(TEXT("BasicAttributeSet"));
	
	AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Death"))
	.AddUObject(this,&AShooting::OnDeadTagChanged);
}

// Called when the game starts or when spawned
void AShooting::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShooting::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooting::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AShooting::PossessedBy(AController* eventControl)//只在服务器调用
{
	Super::PossessedBy(eventControl);
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GrantAbilities(StartingAbilities);
	}
}

void AShooting::OnRep_PlayerState()
{
	
	Super::OnRep_PlayerState();
	if (AbilitySystemComponent)
	{
		
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
	}
}

void AShooting::HandleDeath_Implementation()
{
	AShooting::InitialMeshRelativeTransform = GetMesh()->GetRelativeTransform();
    GetMesh()->SetSimulatePhysics(true);	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();
	
	FVector Impules = GetActorForwardVector() * -1000;
	Impules.Z = 15000;
	GetMesh()->AddImpulseAtLocation(Impules,GetActorLocation());
	
	
}

void AShooting::HandleRespawn_Implementation()
{
	 
	// 2. 先关物理，再改预设（顺序很重要！）
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName(TEXT("mesh")); 
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 恢复检测
	
	GetMesh()->SetRelativeTransform(InitialMeshRelativeTransform);
	//GetMesh()->SetWorldRotation(GetActorRotation());
	
	// 3. 胶囊体恢复碰撞
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Defa"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	if (BasicAttributeSet)
	{
		BasicAttributeSet->SetHealth(BasicAttributeSet->GetMaxHealth());
		// 如果属性是Replicated，SetHealth内部会触发同步
	}
	


	

}
void AShooting::OnDeadTagChanged(const FGameplayTag Callback, int32 NewCount)
{
	if (NewCount >0)
	{
		HandleDeath();
	}	
	else
	HandleRespawn();
}

UAbilitySystemComponent* AShooting::GetAbilitySystemComponent() const
{
	
	return AbilitySystemComponent;
}

TArray<FGameplayAbilitySpecHandle> AShooting::GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant)
{
	
	if (!AbilitySystemComponent|| !HasAuthority())
	{
		
		return TArray<FGameplayAbilitySpecHandle>();
	}
	
	TArray<FGameplayAbilitySpecHandle> AbilityHandles;
	for (TSubclassOf<UGameplayAbility> Ability:AbilitiesToGrant)
	{
		FGameplayAbilitySpecHandle SpecHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(
			Ability,1,-1,this));
		
		AbilityHandles.Add(SpecHandle);
		
	}
	
	SendAbilitiesChangeEvent();
	
	return AbilityHandles;
		
}

void AShooting::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove)
{
	if (!AbilitySystemComponent|| !HasAuthority())
	{
		
		return ;
	}
	
	for (FGameplayAbilitySpecHandle AbilityHandle : AbilityHandlesToRemove)
	{
		AbilitySystemComponent->ClearAbility(AbilityHandle);
		
	}
	SendAbilitiesChangeEvent();
	
	
}

void AShooting::SendAbilitiesChangeEvent()
{
	FGameplayEventData EventData;
	EventData.EventTag = FGameplayTag::RequestGameplayTag(FName("Event.Abilities.Changed"));
	EventData.Instigator=this;
	EventData.Target=this;

		
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, EventData.EventTag, EventData);
		
}

