// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;

UCLASS()
class S1116_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();
    

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    
    
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    USkeletalMeshComponent* MeshComp;
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    TSubclassOf<UDamageType> DamageType;
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    TSubclassOf<UCameraShake> FireCamShake;
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    UParticleSystem* MuzzleEffect;//枪口特效
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    UParticleSystem* ImpactEffect;//血特效
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    UParticleSystem* DefaultImpactEffect;//其他子弹特效
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    UParticleSystem* TraceEffect;//烟雾特效
    
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    FName MuzzleSocketName;
    
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    float BaseDamage;
    
    FTimerHandle TimerHandle_TimeBetweenShots;
    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Weapon")
    float TimeBetweenShots;
    float LastFireTime;
    
public:	
	// Called every frame
    UFUNCTION(BlueprintCallable,Category="Weapon")
    void Fire();
    void StartFire();
    void EndFire();
};
