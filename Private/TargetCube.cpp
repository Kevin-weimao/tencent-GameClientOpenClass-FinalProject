// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetCube.h"
#include "Components/StaticMeshComponent.h"

#include <TestCompiledUE/TestCompiledUEProjectile.h>
#include "GameFramework/PlayerState.h"
#include "TestCompiledUE/TestCompiledUEPlayerController.h"

// Sets default values
ATargetCube::ATargetCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建并设置 StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent; // 将其设为根组件（可选）

	// 确保启用碰撞
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	// 设置物理属性
	StaticMeshComponent->SetSimulatePhysics(true); // 启用物理模拟
	StaticMeshComponent->SetEnableGravity(true);  // 启用重力
	StaticMeshComponent->SetMassOverrideInKg(NAME_None, 10.0f); // 设置质量（单位：kg，可选）

	// 确保碰撞设置正确
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName); // 设置碰撞配置

	// 加载一个静态网格资源
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube"));
	if (MeshAsset.Succeeded())
	{
		StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void ATargetCube::BeginPlay()
{
	Super::BeginPlay();

	// 初始化耐久度
	//Health = 2;
	
	// 为碰撞绑定事件
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ATargetCube::OnHit);
}

// Called every frame
void ATargetCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATargetCube::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& SweepResult)
{
	ATestCompiledUEProjectile* Bullet = Cast<ATestCompiledUEProjectile>(OtherActor); //UE_LOG(LogTemp, Warning, TEXT("OnHit!"));
	if (Bullet != nullptr) {

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController && PlayerController->PlayerState) {
			// 获取蓝图PlayerState实例
			APlayerState* PlayerState = PlayerController->PlayerState;

			// 获取变量的反射信息
			FName VariableName(TEXT("Point")); // 蓝图PlayerState变量名称
			FProperty* Property = PlayerState->GetClass()->FindPropertyByName(VariableName);
			if (Property)
			{
				// 检查变量类型并读取值
				if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
				{
					// 获得积分，并打印当前积分
					int32 PlayerPoint = IntProperty->GetPropertyValue_InContainer(PlayerState);
					PlayerPoint += targetPoint;
					IntProperty->SetPropertyValue_InContainer(PlayerState, PlayerPoint);
					UE_LOG(LogTemp, Log, TEXT("Player Score: %d"), PlayerPoint);

					// 更新UI
					ATestCompiledUEPlayerController* MyController = Cast<ATestCompiledUEPlayerController>(GetWorld()->GetFirstPlayerController());
					if (MyController)
					{
						MyController->UpdateWidgetText(PlayerPoint);
					}
				}
			}
		}

		Health -= 1;
		if (Health > 0){
			ChangeScale(); // 将 Actor 缩放为原来的一半
		}
		else {
			//StaticMeshComponent->OnComponentHit.RemoveAll(this);
			Destroy(); UE_LOG(LogTemp, Log, TEXT("Destroy Cube!"));
		}
	}
}

void ATargetCube::ChangeScale()
{
	FVector NewScale = GetActorScale3D() * ScaleTime;// 定义新的缩放比例
	//FVector NewScale(0.5f, 0.5f, 0.5f);
	SetActorScale3D(NewScale);					// 动态设置缩放
	UE_LOG(LogTemp, Warning, TEXT("ChangeScale!"));
}