// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class USphereComponent;
class ASPowerupActor;

UCLASS()
class S1116_API ASPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickupActor();
    UPROPERTY(VisibleAnywhere,Category="Components")
    USphereComponent* SphereComp;
    
    //texture
    UPROPERTY(VisibleAnywhere,Category="Components")
    UDecalComponent* DecalComp;
    
    UPROPERTY(EditDefaultsOnly,Category="Pickups")
    float CooldownTime;
    
    UPROPERTY(EditInstanceOnly,Category="Pickups")
    TSubclassOf<ASPowerupActor> PowerUpClass;
    
    ASPowerupActor* PowerupInstance;
    
    FTimerHandle TimerHandle_RespawnTimer;
    
    void Respawn();
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
