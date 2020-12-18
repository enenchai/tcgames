// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
    DefaultHealth = 100;
    SetIsReplicated(true);
    CurrentHealth = DefaultHealth;
    HealthPercent = CurrentHealth / DefaultHealth;
}

// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* MyOwner = GetOwner();
		if (MyOwner) {
			MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
		}
	}
	AActor* MyOwner = GetOwner();
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString::SanitizeFloat(HealthPercent));
}



void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor,float Damage,const class UDamageType* DamageType,class AController* InstigatedBy,AActor* DamageCauser)
{
	if (Damage <= 0) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage low:"));
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, DefaultHealth);
	HealthPercent = CurrentHealth / DefaultHealth;

	UE_LOG(LogTemp, Warning, TEXT("Health Changed:%s:"), *FString::SanitizeFloat(CurrentHealth));
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
    
}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = CurrentHealth - OldHealth;
	HealthPercent = CurrentHealth / DefaultHealth;
	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, nullptr, nullptr, nullptr);
}

void USHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || CurrentHealth <= 0.0f) {
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, DefaultHealth);
	HealthPercent = CurrentHealth / DefaultHealth;
	UE_LOG(LogTemp, Log, TEXT("Health Changed:%s(+%s)"), *FString::SanitizeFloat(CurrentHealth), *FString::SanitizeFloat(HealAmount));
	OnHealthChanged.Broadcast(this, CurrentHealth, -HealAmount, nullptr, nullptr, nullptr);
}

float USHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USHealthComponent, CurrentHealth);
	DOREPLIFETIME(USHealthComponent, HealthPercent);
}
void USHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HealthPercent = CurrentHealth / DefaultHealth;
}