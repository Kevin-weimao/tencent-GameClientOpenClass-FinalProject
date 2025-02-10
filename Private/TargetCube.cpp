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

	// ���������� StaticMeshComponent
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent; // ������Ϊ���������ѡ��

	// ȷ��������ײ
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	// ������������
	StaticMeshComponent->SetSimulatePhysics(true); // ��������ģ��
	StaticMeshComponent->SetEnableGravity(true);  // ��������
	StaticMeshComponent->SetMassOverrideInKg(NAME_None, 10.0f); // ������������λ��kg����ѡ��

	// ȷ����ײ������ȷ
	StaticMeshComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName); // ������ײ����

	// ����һ����̬������Դ
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

	// ��ʼ���;ö�
	//Health = 2;
	
	// Ϊ��ײ���¼�
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
			// ��ȡ��ͼPlayerStateʵ��
			APlayerState* PlayerState = PlayerController->PlayerState;

			// ��ȡ�����ķ�����Ϣ
			FName VariableName(TEXT("Point")); // ��ͼPlayerState��������
			FProperty* Property = PlayerState->GetClass()->FindPropertyByName(VariableName);
			if (Property)
			{
				// ���������Ͳ���ȡֵ
				if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
				{
					// ��û��֣�����ӡ��ǰ����
					int32 PlayerPoint = IntProperty->GetPropertyValue_InContainer(PlayerState);
					PlayerPoint += targetPoint;
					IntProperty->SetPropertyValue_InContainer(PlayerState, PlayerPoint);
					UE_LOG(LogTemp, Log, TEXT("Player Score: %d"), PlayerPoint);

					// ����UI
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
			ChangeScale(); // �� Actor ����Ϊԭ����һ��
		}
		else {
			//StaticMeshComponent->OnComponentHit.RemoveAll(this);
			Destroy(); UE_LOG(LogTemp, Log, TEXT("Destroy Cube!"));
		}
	}
}

void ATargetCube::ChangeScale()
{
	FVector NewScale = GetActorScale3D() * ScaleTime;// �����µ����ű���
	//FVector NewScale(0.5f, 0.5f, 0.5f);
	SetActorScale3D(NewScale);					// ��̬��������
	UE_LOG(LogTemp, Warning, TEXT("ChangeScale!"));
}