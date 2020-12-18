// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/SHealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "SCharacter.h"
//#include "TimerManager.h"
#include "Sound/SoundCue.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
//
    MeshComp->SetCanEverAffectNavigation(false);
    RootComponent = MeshComp;
//    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
//    SphereComp->SetSphereRadius(200.0f);
//    SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//    SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
//    SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
//    SphereComp->SetupAttachment(RootComponent);
//
//    HelathComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
//    HelathComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);
//
//
//
    MeshComp->SetSimulatePhysics(true);
//
    MovementForce = 1000;
    RequiredDistanceToTarget = 100;
//
    bExploded = false;
//    bStartedSelfDestruction = false;
//    ExplosionRadius = 200;
//    ExplosionDamage = 40;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
    Super::BeginPlay();
    if (Role == ROLE_Authority) {
        GetNextPathPoint();
    }

}

//void ASTrackerBot::HandleTakeDamage(USHealthComponent * HealthComponent, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
//{
//    if (MatInst == nullptr)
//        MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
//
//    if (MatInst) {
//        MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
//    }
//
//    if (Health <= 0) {
//        SelfDestruct();
//    }
//
//    UE_LOG(LogTemp, Log, TEXT("Health is %s of %s"), *FString::SanitizeFloat(Health), *GetName());
//}
//
FVector ASTrackerBot::GetNextPathPoint()
{
    ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

    UNavigationPath* NavPath = nullptr;
    //NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);
    if (NavPath&&NavPath->PathPoints.Num() > 1) {
        return NavPath->PathPoints[1];
    }
    return GetActorLocation();
}
//
//void ASTrackerBot::SelfDestruct()
//{
//    if (bExploded) {
//        return;
//    }
//
//    bExploded = true;
//
//
//    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
//    UGameplayStatics::PlaySoundAtLocation(this, ExplodedSound, GetActorLocation());
//
//    MeshComp->SetVisibility(false, true);
//    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//
//    if (Role == ROLE_Authority) {
//        TArray<AActor*> IgnoreActors;
//        IgnoreActors.Add(this);
//        UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionRadius, nullptr, IgnoreActors, this, GetInstigatorController(), true);
//
//        DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 2.0f, 0, 1.0f);
//
//        SetLifeSpan(2.0f);
//    }
//}
//
//void ASTrackerBot::DamageSelf()
//{
//    UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), this, nullptr);
//}
//
//// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (Role == ROLE_Authority&&!bExploded)
    {
        float DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();

        if (DistanceToTarget <= RequiredDistanceToTarget) {
            NextPathPoint = GetNextPathPoint();
        }
        else {
            FVector ForceDirection = NextPathPoint - GetActorLocation();
            ForceDirection.Normalize();
            ForceDirection *= MovementForce;
            MeshComp->AddForce(ForceDirection, NAME_None, true);
        }
    }

}
//
//// Called to bind functionality to input
//void ASTrackerBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//    Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}
//
//void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor) {
//
//    Super::NotifyActorBeginOverlap(OtherActor);
//    if (!bStartedSelfDestruction&&!bExploded) {
//
//        bStartedSelfDestruction = true;
//        ASCharacter* PlayerPawn = Cast<ASCharacter>(OtherActor);
//
//        if (PlayerPawn) {
//
//            if(Role==ROLE_Authority)
//            GetWorldTimerManager().SetTimer(TimeHandle_SelfDamage, this, &ASTrackerBot::DamageSelf, 0.5f, true, 0.0f);
//
//            UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
//        }
//    }
//}
//
//
