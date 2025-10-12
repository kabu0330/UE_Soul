// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE_Soul/Interfaces/Soul_Interact.h"
#include "Soul_PickupItem.generated.h"

class ASoul_Equipment;

UCLASS()
class UE_SOUL_API ASoul_PickupItem : public AActor, public ISoul_Interact
{
	GENERATED_BODY()

public:
	ASoul_PickupItem();
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	FORCEINLINE void SetEquipmentClass(const TSubclassOf<ASoul_Equipment>& NewEquipmentClass) {EquipmentClass = NewEquipmentClass;}
	
	/** Interface */
	virtual void Interact(AActor* InteractActor) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PickupItem")
	TSubclassOf<ASoul_Equipment> EquipmentClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PickupItem")
	TObjectPtr<UStaticMeshComponent> Mesh;
	
private:

};
