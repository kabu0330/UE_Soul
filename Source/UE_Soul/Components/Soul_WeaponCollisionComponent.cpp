// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_WeaponCollisionComponent.h"


USoul_WeaponCollisionComponent::USoul_WeaponCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}


void USoul_WeaponCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USoul_WeaponCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
												   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsCollisionEnabled)
	{
		CollisionTrace();
	}
}

void USoul_WeaponCollisionComponent::TurnOnCollision()
{
	AlreadyHitActors.Empty();
	bIsCollisionEnabled = true;
}

void USoul_WeaponCollisionComponent::TurnOffCollision()
{
	bIsCollisionEnabled = false;
}

void USoul_WeaponCollisionComponent::SetWeaponMesh(UPrimitiveComponent* MeshComponent)
{
	WeaponMesh = MeshComponent;
}

void USoul_WeaponCollisionComponent::AddIgnoredActor(AActor* Actor)
{
	IgnoreActors.Add(Actor);
}

void USoul_WeaponCollisionComponent::RemoveIgnoredActor(AActor* Actor)
{
	IgnoreActors.Remove(Actor);
}

bool USoul_WeaponCollisionComponent::CanHitActor(AActor* Actor) const
{
	return AlreadyHitActors.Contains(Actor) == false;
}

void USoul_WeaponCollisionComponent::CollisionTrace()
{
	TArray<FHitResult> OutHits;

	const FVector Start = WeaponMesh->GetSocketLocation(TraceStartSocketName);
	const FVector End = WeaponMesh->GetSocketLocation(TraceEndSocketName);

	bool const bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetOwner(),
		Start,
		End,
		TraceRadius,
		TraceObjectTypes,
		false,
		IgnoreActors,
		DrawDebugType,
		OutHits,
		true
	);

	if (bHit)
	{
		for (const FHitResult& Hit : OutHits)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor == nullptr)
			{
				continue;
			}
			// 이전 프레임에서 이미 충돌한 액터가 아니라면
			if (CanHitActor(HitActor))
			{
				AlreadyHitActors.Add(HitActor);

				if (OnHitActor.IsBound()) // 바인딩된 함수가 있다면
				{
					OnHitActor.Broadcast(Hit);
				}
			}
		}
	}
}

