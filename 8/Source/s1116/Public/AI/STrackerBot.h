// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class S1116_API ASTrackerBot : public APawn
{
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ASTrackerBot();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere,Category="Components")
    UStaticMeshComponent* MeshComp;

//    UFUNCTION()
//    void HandleTakeDamage(USHealthComponent * HealthComponent, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);
//
//    UPROPERTY(VisibleAnywhere, Category = "Components")
//    USHealthComponent* HelathComp;
//
//    UMaterialInstanceDynamic* MatInst;
//
    FVector GetNextPathPoint();
//
    FVector NextPathPoint;
//
    UPROPERTY(EditDefaultsOnly,Category="TrackerBot")
    float MovementForce;
//
    UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
    float RequiredDistanceToTarget;
//
//    void SelfDestruct();
//
//    UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
//    UParticleSystem* ExplosionEffect;
//
//    UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
//    float ExplosionDamage;
//
//    UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
//    float ExplosionRadius;
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
//        TSubclassOf <UDamageType > DamageType;
//
    bool bExploded;
//
//    UPROPERTY(VisibleAnywhere, Category = "Components")
//    USphereComponent* SphereComp;
//
//    void DamageSelf();
//
//    FTimerHandle TimeHandle_SelfDamage;
//
//    bool bStartedSelfDestruction;
//
//    UPROPERTY(EditDefaultsOnly,Category="Sound")
//    USoundCue* SelfDestructSound;
//
//    UPROPERTY(EditDefaultsOnly, Category = "Sound")
//    USoundCue* ExplodedSound;


public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

//    // Called to bind functionality to input
//    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
//    
//    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
