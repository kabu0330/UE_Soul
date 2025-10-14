// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_Enemy.h"

#include "NavigationSystemTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "UE_Soul/Components/Soul_AttributeComponent.h"
#include "UE_Soul/Components/Soul_StateComponent.h"
#include "UE_Soul/Data/Soul_GameplayTags.h"


ASoul_Enemy::ASoul_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 카메라와 충돌은 무시
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<USoul_AttributeComponent>("AttributeComponent");
	StateComponent = CreateDefaultSubobject<USoul_StateComponent>("StateComponent");

	AttributeComponent->OnDeath.AddUObject(this, &ThisClass::OnDeath);

	// 타게팅
	TargetingSphereComponent = CreateDefaultSubobject<USphereComponent>("TargetingSphereComponent");
	TargetingSphereComponent->SetupAttachment(GetRootComponent());
	TargetingSphereComponent->SetCollisionObjectType(COLLISION_OBJECT_TARGETING);
	TargetingSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TargetingSphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // 비지빌리티 트레이스 채널만 켜고 나머지는 다 무시

	LockOnWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("LockOnWidgetComponent");
	LockOnWidgetComponent->SetupAttachment(GetRootComponent());
	LockOnWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	LockOnWidgetComponent->SetDrawSize(FVector2D(30.f, 30.f));
	LockOnWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // 2D
	LockOnWidgetComponent->SetVisibility(false); // 안 보이게
}

void ASoul_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoul_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASoul_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ASoul_Enemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage =  Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	check(AttributeComponent);

	// 데미지 적용
	AttributeComponent->TakeDamageAmount(ActualDamage);
	GEngine->AddOnScreenDebugMessage(6, 1.5f, FColor::Cyan, FString::Printf(TEXT("Damage : %f"), ActualDamage));

	// UGameplayStatics::ApplyPointDamage 함수를 호출하면 FPointDamageEvent::ClassID를 통해 PointDamage를 입었다는 사실을 체크할 수 있다. 
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);

		// 데미지 방향
		FVector ShotDirection = PointDamageEvent->ShotDirection;

		// 히트 위치 (에너미 충돌 위치)
		FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint;

		// 히트 방향
		FVector ImpactDirection = PointDamageEvent->HitInfo.ImpactNormal;

		// 히트한 객체의 위치 (부딪힌 무기의 위치)
		FVector HitLocation = PointDamageEvent->HitInfo.Location;

		ImpactEffect(ImpactPoint);
		HitReaction(EventInstigator->GetPawn());
	}
	
	return ActualDamage;
}

void ASoul_Enemy::OnDeath()
{
	if (UCapsuleComponent* CapsuleComp = GetCapsuleComponent())
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		// 물리 시뮬레이션 적용
		MeshComp->SetCollisionProfileName("Ragdoll");
		MeshComp->SetSimulatePhysics(true);
		
		MeshComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
}

void ASoul_Enemy::OnTargeted(bool bTarget)
{
	check(LockOnWidgetComponent);
	LockOnWidgetComponent->SetVisibility(bTarget);
}

bool ASoul_Enemy::CanBeTargeted()
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(Soul_GameplayTag::Character_State_Death);
	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ASoul_Enemy::ImpactEffect(const FVector& Location)
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Location);
	}

	if (ImpactParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactParticle, Location);
	}
}

void ASoul_Enemy::HitReaction(const AActor* Attacker)
{
	if (UAnimMontage* HitReactAnimMontage = GetHitReactAnimation(Attacker))
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Reaction!!!!"));
		float DelaySeconds = PlayAnimMontage(HitReactAnimMontage);
	}
}

UAnimMontage* ASoul_Enemy::GetHitReactAnimation(const AActor* Attacker) const
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Attacker->GetActorLocation());
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(GetActorRotation(), LookAtRotation);
	const float DeltaZ = DeltaRotation.Yaw;

	EHitDirection HitDirection = EHitDirection::Front;

	if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -45.f, 45.f))
	{
		HitDirection = EHitDirection::Front;
		UE_LOG(LogTemp, Log, TEXT("Hit Direction : Front"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 45.f, 135.f))
	{
		HitDirection = EHitDirection::Left;
		UE_LOG(LogTemp, Log, TEXT("Hit Direction : Left"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, 135.f, 180.f)
		|| UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -180.f, -135.f))
	{
		HitDirection = EHitDirection::Back;
		UE_LOG(LogTemp, Log, TEXT("Hit Direction : Back"));
	}
	else if (UKismetMathLibrary::InRange_FloatFloat(DeltaZ, -135.f, -45.f))
	{
		HitDirection = EHitDirection::Right;
		UE_LOG(LogTemp, Log, TEXT("Hit Direction : Right"));
	}

	UAnimMontage* SelectedMontage = nullptr;
	switch (HitDirection)
	{
		case EHitDirection::Front:
			SelectedMontage = HitReactAnimFront;
			break;
		case EHitDirection::Left:
			SelectedMontage = HitReactAnimLeft;
			break;
		case EHitDirection::Back:
			SelectedMontage = HitReactAnimBack;
			break;
		case EHitDirection::Right:
			SelectedMontage = HitReactAnimRight;
			break;
		default:
		break;
	}
	return SelectedMontage;
}

