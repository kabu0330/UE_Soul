// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Soul_Equipment.generated.h"

UCLASS()
class UE_SOUL_API ASoul_Equipment : public AActor
{
	GENERATED_BODY()

public:
	ASoul_Equipment();
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void EquipItem() {}
	virtual void UnequipItem() {}
	virtual void AttachToOwner(FName SocketName);
	
	UPROPERTY(EditDefaultsOnly, Category = "Equipment|Mesh")
	TObjectPtr<UStaticMesh> MeshAsset;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment|Mesh")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
protected:
	virtual void BeginPlay() override;

private:
	
};
