// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_PickupItem.h"

#include "UE_Soul/Data/Soul_Define.h"
#include "UE_Soul/Equip/Soul_Equipment.h"


ASoul_PickupItem::ASoul_PickupItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Mesh->SetSimulatePhysics(true); // 공중에 떠 있지 않고 바닥에 떨어뜨리기
	Mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Mesh->SetCollisionObjectType(COLLISION_OBJECT_INTERACTION);
}

void ASoul_PickupItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoul_PickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoul_PickupItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (EquipmentClass)
	{
		if (ASoul_Equipment* CDO = EquipmentClass->GetDefaultObject<ASoul_Equipment>())
		{
			Mesh->SetStaticMesh(CDO->MeshAsset);
		}
	}
}

void ASoul_PickupItem::Interact(AActor* InteractActor)
{
	GEngine->AddOnScreenDebugMessage(5, 1.5f, FColor::Cyan, FString::Printf(TEXT("Pickup Item")));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = InteractActor;

	ASoul_Equipment* SpawnItem = GetWorld()->SpawnActor<ASoul_Equipment>(EquipmentClass, GetActorTransform(), SpawnParams);
	if (SpawnItem)
	{
		SpawnItem->EquipItem();
		Destroy();
	}
}

