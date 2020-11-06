// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

//课时16
void ASWeapon::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); //设置EyeLocation和EyeRotation 从MyOwner里得到
		FVector ShotDirection = EyeRotation.Vector();
		FVector EndLocation = EyeLocation + ShotDirection * 10000;
		FCollisionQueryParams Params; //检测碰撞的细节
		Params.AddIgnoredActor(MyOwner); //忽略碰撞到人
		Params.AddIgnoredActor(this); //忽略碰撞到枪
		Params.bTraceComplex = true; //碰撞的物体具体到面
		FHitResult OutHit;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, EyeLocation, EndLocation, ECC_Visibility, Params))
		{
			//1 2 射线起始位置 3 射线终止位置 4 需要跟哪些物体检测 5 碰撞细节

			AActor* HitActor = OutHit.GetActor(); //打到谁身上
			

			//造成伤害
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, OutHit, MyOwner->GetInstigatorController(), this, DamageType);
			//1 HitActor 打谁 2 基础伤害 3 发射方向 4 OutHit 5 伤害来源 6 伤害造成的武器 7 伤害类型

			WScore++;
			UE_LOG(LogTemp, Warning, TEXT("666"));
		}
		
		DrawDebugLine(GetWorld(), EyeLocation, EndLocation, FColor::White, false, 1.0, 0, 1);
		//1 world 2 起始位置 3 终点 4 画线的颜色 5 是否需要持续存在 6 存在的时间 7 优先级 8 线宽度
	}
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

