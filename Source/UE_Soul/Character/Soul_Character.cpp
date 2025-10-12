// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul_Character.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UE_Soul/Components/Soul_AttributeComponent.h"
#include "UE_Soul/Components/Soul_CombatComponent.h"
#include "UE_Soul/Components/Soul_StateComponent.h"
#include "UE_Soul/Data/Soul_GameplayTags.h"
#include "UE_Soul/Equip/Soul_Weapon.h"
#include "UE_Soul/Interfaces/Soul_Interact.h"
#include "UE_Soul/UI/Soul_PlayerOverlay.h"


ASoul_Character::ASoul_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // 감속 속도
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SetRelativeRotation(FRotator(-30.0f, 0.f, 0.0f));
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AttributeComponent = CreateDefaultSubobject<USoul_AttributeComponent>("AttributeComponent");
	StateComponent = CreateDefaultSubobject<USoul_StateComponent>("StateComponent");
	CombatComponent = CreateDefaultSubobject<USoul_CombatComponent>("CombatComponent");
}

void ASoul_Character::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerOverlayClass)
	{
		PlayerOverlay = CreateWidget<USoul_PlayerOverlay>(GetWorld(), PlayerOverlayClass);
		if (PlayerOverlay)
		{
			PlayerOverlay->AddToViewport();
		}
	}
	
}

void ASoul_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(0, 1.5f, FColor::Cyan, FString::Printf(TEXT("Stamina : %f"), AttributeComponent->GetBaseStamina()));
	GEngine->AddOnScreenDebugMessage(2, 1.5f, FColor::Cyan, FString::Printf(TEXT("MaxStamina : %f"), AttributeComponent->GetMaxStamina()));
}

void ASoul_Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASoul_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASoul_Character::Move);

		// 질주
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ASoul_Character::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ASoul_Character::StopSprint);

		// 구르기
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ASoul_Character::Rolling);

		// 장비 아이템 줍기
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASoul_Character::Interact);

		// 전투 상태 전환
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ASoul_Character::ToggleCombat);
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoul_Character::Look);
	} 
}

void ASoul_Character::Move(const FInputActionValue& Value)
{
	// 이동이 가능한 상태가 아니면 리턴
	check(StateComponent);
	if (StateComponent->MovementInputEnabled() == false)
	{
		return;
	}
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardVector, MovementVector.X);
		AddMovementInput(RightVector, MovementVector.Y);
	}
}

void ASoul_Character::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

bool ASoul_Character::IsMoving() const
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->Velocity.Size2D() > 3.f && GetCharacterMovement()->GetCurrentAcceleration() != FVector::Zero();
	}
	return false;
}

bool ASoul_Character::CanToggleCombat() const
{
	check(StateComponent);

	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(Soul_GameplayTag::Character_State_Attacking);
	CheckTags.AddTag(Soul_GameplayTag::Character_State_Rolling);
	CheckTags.AddTag(Soul_GameplayTag::Character_State_GeneralAction);

	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false;
}

void ASoul_Character::Sprinting()
{
	// 최소 스태미나보다 많고, 이동 중일 때만 질주할 수 있다.
	if (AttributeComponent->CheckHasEnoughStamina(5.f) && IsMoving())
	{
		// 질주 중에는 스태미나 회복을 멈춘다.
		AttributeComponent->ToggleStaminaRegeneration(false);
		
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		AttributeComponent->DecreaseStamina(0.1f);
	}
	else
	{
		StopSprint();
	}
}

void ASoul_Character::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	AttributeComponent->ToggleStaminaRegeneration(true);
}

void ASoul_Character::Rolling()
{
	check(AttributeComponent);
	check(StateComponent);

	if (AttributeComponent->CheckHasEnoughStamina(15.f))
	{
		// 스태미나 재충전 멈추고
		AttributeComponent->ToggleStaminaRegeneration(false);

		// 이동 멈추고
		StateComponent->ToggleMovementInput(false);

		// 스태미나 차감
		AttributeComponent->DecreaseStamina(15.0f);

		// 구르기 애니메이션 재생
		PlayAnimMontage(RollingMontage);

		// 상태 설정
		StateComponent->SetState(Soul_GameplayTag::Character_State_Rolling);

		// 1.5초 뒤에 스태미나 재충전 시작
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f);
	}
}

void ASoul_Character::Interact()
{
	FHitResult HitResult;
	const FVector Start = GetActorLocation();
	const FVector End = Start;
	constexpr float Radius = 100.0f; // 감지 영역의 크기

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(COLLISION_OBJECT_INTERACTION));

	TArray<AActor*> ActorsToIgnore;

	bool bHit = UKismetSystemLibrary::SphereTraceSingleForObjects(
		this,
		Start,
		End,
		Radius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
		);

	if (bHit)
	{
		if (AActor* HitActor = HitResult.GetActor())
		{
			if (ISoul_Interact* Interaction = Cast<ISoul_Interact>(HitActor))
			{
				Interaction->Interact(this);
			}
		}
	}
}

void ASoul_Character::ToggleCombat()
{
	check(CombatComponent);
	check(StateComponent);

	// 무기를 가지고 있다면
	if (const ASoul_Weapon* Weapon = CombatComponent->GetMainWeapon())
	{
		if (CanToggleCombat()) // 전투 모드로 전환이 가능한 상태인지
		{
			StateComponent->SetState(Soul_GameplayTag::Character_State_GeneralAction);
			
			if (CombatComponent->IsCombatEnabled()) // 전투 모드에서 토글 키를 눌렀으면 비전투 상태로 
			{
				PlayAnimMontage(Weapon->GetMontageForTag(Soul_GameplayTag::Character_Action_Unequip));
			}
			else // 비전투 모드에서 토글 키를 눌렀으면 전투 상태로
			{
				PlayAnimMontage(Weapon->GetMontageForTag(Soul_GameplayTag::Character_Action_Equip));
			}
		}
	}
}


