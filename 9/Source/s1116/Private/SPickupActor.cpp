// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickupActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "SPowerupActor.h"
#include "SCharacter.h"
#include "TimerManager.h"

// Sets default values
ASPickupActor::ASPickupActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    SetReplicates(true);
    
    SphereComp= CreateDefaultSubobject<USphereComponent>(TEXT("ShpereComp"));
    SphereComp->SetSphereRadius(75.0f);
    SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    RootComponent = SphereComp;

    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
    DecalComp->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
    DecalComp->DecalSize = FVector(64, 75, 75);

    DecalComp->SetupAttachment(RootComponent);
//
    CooldownTime = 5.0f;
}

void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
    if(Role==ROLE_Authority)
    {
        Respawn();
    }
}

// Called every frame
void ASPickupActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASPickupActor::Respawn()
{
    if (PowerUpClass == nullptr)
    {
        //UE_LOG(LogTemp, Warning, TEXT("PowerUpClass is nullptr in %s"), *GetName());
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    PowerupInstance= GetWorld()->SpawnActor<ASPowerupActor>(PowerUpClass,GetTransform(),SpawnParams);
}

void ASPickupActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
    ASWeapon* ThisWeapon = PlayerPawn->GetWeapon();
    if (PlayerPawn)
    {
        if (PowerupInstance&&Role==ROLE_Authority)
        {
            if (PowerupInstance->GetFName().ToString().Contains("Accelerate"))
            {
                PlayerPawn->bGetEnergy = true;
                PlayerPawn->PowerupInterval = PowerupInstance->PowerupInterval;
                PlayerPawn->TempEnergy= PowerupInstance->PowerupInterval;
            }
            if (PowerupInstance->GetFName().ToString().Contains("Immu"))
            {
                PlayerPawn->BeImmued();
                PlayerPawn->PowerupInterval = PowerupInstance->PowerupInterval;
                PlayerPawn->TempEnergy= PowerupInstance->PowerupInterval;
            }
            if (PowerupInstance->GetFName().ToString().Contains("Recover"))
            {
                PlayerPawn->PowerupInterval = PowerupInstance->PowerupInterval;
                PlayerPawn->TempEnergy= PowerupInstance->PowerupInterval;
            }
            PowerupInstance->ToActive(OtherActor,ThisWeapon);
            PowerupInstance = nullptr;

            GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickupActor::Respawn, CooldownTime);
        }
    }
}
