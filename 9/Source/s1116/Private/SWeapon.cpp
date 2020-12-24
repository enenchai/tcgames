// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"

#define SURFACETYPE_FleshDefault SurfaceType1
#define SURFACETYPE_FleshVulnerable SurfaceType2
#define COLLISION_WEAPON ECC_GameTraceChannel1

static int32 DebugWeaponDrawing = 0;

FAutoConsoleVariableRef CVARDebugWeaponDrawing
{
    TEXT("Shoot.DebugWeapons"),
    DebugWeaponDrawing,
    TEXT("Draw Debug Lines For Weapons"),
    ECVF_Cheat
};

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    MuzzleSocketName = "MuzzleSocket";
    BaseDamage = 20;
    TimeBetweenShots = 0.1f;
    
    static ConstructorHelpers::FObjectFinder<UParticleSystem> ME(TEXT("'/Game/WeaponEffects/Muzzle/P_Muzzle_Large'"));
    MuzzleEffect = ME.Object;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> IE(TEXT("'/Game/WeaponEffects/BloodImpact/P_blood_splash_02'"));
    FleshImpactEffect = IE.Object;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> DIE(TEXT("'/Game/WeaponEffects/GenericImpact/P_RifleImpact'"));
    ImpactEffect = DIE.Object;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> TE(TEXT("'/Game/WeaponEffects/BasicTracer/P_SmokeTrail'"));
    TraceEffect = TE.Object;
    
    SetReplicates(true);
}

void ASWeapon::Fire()
{
    if (Role < ROLE_Authority) {
        ServerFire();
    }
    
    AActor* MyOwner = GetOwner();
    if(MyOwner)
    {
        FVector EyeLocation;
        FRotator EyeRotation;
        MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);//获得EyeLocation和EyeRotation的值
        
        FVector ShotDirection = EyeRotation.Vector();
        FVector EndLocation = EyeLocation + ShotDirection*10000;//子弹终点
        FVector TracerEndPoint = EndLocation;//子弹烟雾路径终点
        FHitResult OutHit;
        
        
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(MyOwner);
		Params.AddIgnoredActor(this);
		Params.bTraceComplex = true;
		Params.bReturnPhysicalMaterial = true;
		EPhysicalSurface SurfaceType;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, EyeLocation, EndLocation, COLLISION_WEAPON, Params)) {

			TracerEndPoint = OutHit.ImpactPoint;

			//TODO
			AActor* HitActor = OutHit.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get());


			float ActualDamage = BaseDamage;

			if (SurfaceType == SURFACETYPE_FleshVulnerable) {
				ActualDamage = BaseDamage * 4;
			}

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, OutHit, MyOwner->GetInstigatorController(), this, DamageType);

			PlayImpactEffect(SurfaceType, OutHit.ImpactPoint);

		}
		if (DebugWeaponDrawing > 0)
			DrawDebugLine(GetWorld(), EyeLocation, EndLocation, FColor::White, false, 1.0, 0, 1);

		PlayFireEffects(TracerEndPoint);

		if (Role == ROLE_Authority) {
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}

		LastFireTime = GetWorld()->TimeSeconds;
	}
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::OnRep_HitScanTrace()
{
	PlayFireEffects(HitScanTrace.TraceTo);
	PlayImpactEffect(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASWeapon::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{

	UParticleSystem* SelectedEffect = nullptr;

	switch (SurfaceType)
	{
	case SurfaceType1:
	case SurfaceType2:
		SelectedEffect = FleshImpactEffect;
		break;
	default:
            SelectedEffect = nullptr;
            //ImpactEffect;
		break;
	}

	if (SelectedEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = ImpactPoint - MuzzleLocation;
		ShotDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, ImpactPoint, ShotDirection.Rotation());
	}
}

void ASWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.0f);

	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ASWeapon::EndFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void ASWeapon::PlayFireEffects(FVector TracerEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TraceEffect) {

		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);

		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, MuzzleLocation);

		if (TracerComp) {
			TracerComp->SetVectorParameter("BeamEnd", TracerEndPoint);
		}

	}

	APawn* MyOwner = Cast<APawn>(GetOwner());

	if (MyOwner) {
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());
		if (PC) {
			PC->ClientPlayCameraShake(FireCamShake);
		}
	}
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}

