// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_Equipment.h"

#include "GameFramework/Character.h"


ASoul_Equipment::ASoul_Equipment()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void ASoul_Equipment::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoul_Equipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoul_Equipment::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (MeshAsset)
	{
		Mesh->SetStaticMesh(MeshAsset);
	}
}

void ASoul_Equipment::AttachToOwner(FName SocketName)
{
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (USkeletalMeshComponent* CharacterMesh = OwnerCharacter->GetMesh())
		{
			AttachToComponent(CharacterMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), SocketName);
		}
	}
}

