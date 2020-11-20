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

#define SUR_TYPE_FLESH SurfaceType1
#define SUR_TYPE_VUL SurfaceType2

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
    ImpactEffect = IE.Object;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> DIE(TEXT("'/Game/WeaponEffects/GenericImpact/P_RifleImpact'"));
    DefaultImpactEffect = DIE.Object;
    static ConstructorHelpers::FObjectFinder<UParticleSystem> TE(TEXT("'/Game/WeaponEffects/BasicTracer/P_SmokeTrail'"));
    TraceEffect = TE.Object;
    
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Fire()
{
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
        
        
        FCollisionQueryParams Params;//参数 发射子弹忽略哪些物体
        Params.AddIgnoredActor(MyOwner);//忽略人
        Params.AddIgnoredActor(this);//忽略枪
        Params.bTraceComplex = true;//三角面为判断命中最小单位
        Params.bReturnPhysicalMaterial = true;
        
        if(GetWorld()->LineTraceSingleByChannel(OutHit,EyeLocation,EndLocation,ECC_Visibility,Params))
        {
            AActor* HitActor = OutHit.GetActor();//伤害来源
            
            float DamageToken = 0;
            
            
            TracerEndPoint = OutHit.ImpactPoint;
            
            EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get());
            UParticleSystem* SelectedEffect = nullptr;
            switch(SurfaceType)
            {
                case SUR_TYPE_FLESH:
                    SelectedEffect = ImpactEffect;
                    DamageToken = BaseDamage;
                    break;
                case SUR_TYPE_VUL:
                    SelectedEffect = ImpactEffect;
                    DamageToken = BaseDamage*4;
                    break;
                default:
                    SelectedEffect = DefaultImpactEffect;
                    break;
            }
            
            UGameplayStatics::ApplyPointDamage(HitActor,DamageToken,ShotDirection,OutHit,MyOwner->GetInstigatorController(),
                                               this,DamageType);
            
            
            //血特效
            if(SelectedEffect)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),SelectedEffect,OutHit.ImpactPoint,OutHit.ImpactNormal.Rotation());
            }
        }
        
        if(DebugWeaponDrawing)
        {
            DrawDebugLine(GetWorld(),EyeLocation,EndLocation,FColor::White,false,1.0,0,1);
            //4 颜色 5 是否需要持续存在 6 存在时间 7 优先级 8 宽度
        }
        //枪口特效
        if(MuzzleEffect)
        {
            UGameplayStatics::SpawnEmitterAttached(MuzzleEffect,MeshComp,MuzzleSocketName);
        }
        
        //烟雾特效 不管是否击中目标都有烟雾特效
        if(TraceEffect)
        {
            FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);//枪口
            
            UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),TraceEffect,MuzzleLocation);
            if(TracerComp)
            {
                TracerComp->SetVectorParameter("BeamEnd",TracerEndPoint);
            }
            //3 起点 枪口位置
        }
        
        LastFireTime = GetWorld()->TimeSeconds;
        
        //摄像机抖动
        APawn* MyOwner2 = Cast<APawn>(GetOwner());
        if(MyOwner2)
        {
            APlayerController* PC = Cast<APlayerController>(MyOwner2->GetController());
            if(PC)
            {
                PC->ClientPlayCameraShake(FireCamShake);
            }
        }
    }
}

void ASWeapon::StartFire()
{
    float FirstDelay = LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds;
    FirstDelay = FirstDelay>0?FirstDelay:0;
    GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots,this,&ASWeapon::Fire,TimeBetweenShots,true,FirstDelay);
    
}

void ASWeapon::EndFire()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}
