// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature,USHealthComponent*, HealthComponent,float,Health,float, HealthDelta,const class UDamageType*, DamageType,class AController*, InstigatedBy,AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class S1116_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HealthComponent")
		float DefaultHealth;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadWrite, Category = "HealthComponent")
		float CurrentHealth;

	UFUNCTION()
		void OnRep_Health(float OldHealth);

	UFUNCTION()
		void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "HealthComponent")
		void Heal(float HealAmount);

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Health")
		float HealthPercent;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnHealthChangedSignature OnHealthChanged;

	float GetCurrentHealth() const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
