// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class S1116_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly,Category="Powerups")
    int32 TotalUpdateTimes;
    
    int32 CurrentUpdateTimes;
    
    UFUNCTION()
    void OnTickPowerup();
    
    FTimerHandle TimerHandle_PowerupTick;
    
    UPROPERTY(ReplicatedUsing=OnRep_PowerupActive)
    bool bIsPowerupActive;
    
    UFUNCTION()
    void OnRep_PowerupActive();
    
    UFUNCTION(BlueprintImplementableEvent,Category="Powerups")
    void OnPowerupStateChanged(bool bNewActive);
    
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    
    void ToActive(AActor* OtherActor,ASWeapon* ThisWeapon);//激活
    
    UFUNCTION(BlueprintImplementableEvent,Category = "Powerups")
    void OnActived(AActor* OtherActor,ASWeapon* ThisWeapon);//激活后
    UFUNCTION(BlueprintImplementableEvent,Category = "Powerups")
    void OnExpired();//失效后
    UFUNCTION(BlueprintImplementableEvent,Category = "Powerups")
    void OnUpdated();//更新后
    
    UPROPERTY(Replicated,EditDefaultsOnly,Category="Powerups")
    float PowerupInterval;//更新间隔
};
