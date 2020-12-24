// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPowerupActor::ASPowerupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    PowerupInterval = 0.0f;
    TotalUpdateTimes = 0;
    SetReplicates(true);
    bIsPowerupActive = false;
}

// Called when the game starts or when spawned
void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPowerupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPowerupActor::ToActive(AActor* OtherActor,ASWeapon* ThisWeapon)
{
    OnActived(OtherActor,ThisWeapon);
    bIsPowerupActive = true;
    OnPowerupStateChanged(bIsPowerupActive);

    if (PowerupInterval > 0.0f)
    {
        GetWorldTimerManager().SetTimer(TimerHandle_PowerupTick,this,&ASPowerupActor::OnTickPowerup,PowerupInterval,true);
    }
    else
    {
        //only call once
        OnTickPowerup();
    }
}

void ASPowerupActor::OnTickPowerup()
{
    CurrentUpdateTimes++;
    OnUpdated();

    if (CurrentUpdateTimes >= TotalUpdateTimes)
    {
        OnExpired();
        bIsPowerupActive = false;
        OnPowerupStateChanged(bIsPowerupActive);
        GetWorldTimerManager().ClearTimer(TimerHandle_PowerupTick);
    }
}

void ASPowerupActor::OnRep_PowerupActive()
{
    OnPowerupStateChanged(bIsPowerupActive);
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
    
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASPowerupActor, bIsPowerupActive);
    DOREPLIFETIME(ASPowerupActor, PowerupInterval);
}
