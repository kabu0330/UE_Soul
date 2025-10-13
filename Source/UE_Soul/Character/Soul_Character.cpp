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
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASoul_Character::Look);

		// 질주
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Triggered, this, &ASoul_Character::Sprinting);
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Completed, this, &ASoul_Character::StopSprint);

		// 구르기
		EnhancedInputComponent->BindAction(SprintRollingAction, ETriggerEvent::Canceled, this, &ASoul_Character::Rolling);

		// 장비 아이템 줍기
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ASoul_Character::Interact);

		// 전투 상태 전환
		EnhancedInputComponent->BindAction(ToggleCombatAction, ETriggerEvent::Started, this, &ASoul_Character::ToggleCombat);

		// 공격
		// 무기를 들고 있지 않으면 즉시 무기를 꺼내는 애니메이션 재생
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASoul_Character::AutoToggleCombat);
		// 마우스 버튼을 떼면 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Canceled, this, &ASoul_Character::Attack);
		// 트리거 조건(마우스 버튼을 길게)이면 특수 공격
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASoul_Character::SpecialAttack);
		// 특수 공격
		EnhancedInputComponent->BindAction(HeavyAttackAction, ETriggerEvent::Started, this, &ASoul_Character::HeavyAttack);
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

		bSprinting = true;
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

	bSprinting = false;
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

void ASoul_Character::AutoToggleCombat()
{
	check(CombatComponent);
	if (false == CombatComponent->IsCombatEnabled())
	{
		ToggleCombat();
	}
}

bool ASoul_Character::CanPerformAttack(const FGameplayTag& AttackTypeTag) const
{
	check(StateComponent);
	check(CombatComponent);
	check(AttributeComponent);

	// 무기를 들고 있는지?
	if (IsValid(CombatComponent->GetMainWeapon()) == false)
	{
		return false;
	}

	// 공격을 할 수 없는 동작들을 여기서 정의
	FGameplayTagContainer CheckTags;
	CheckTags.AddTag(Soul_GameplayTag::Character_State_Rolling);
	CheckTags.AddTag(Soul_GameplayTag::Character_State_Attacking);

	// 공격에 필요한 스태미나 비용 가져오기
	const float StaminaCost = CombatComponent->GetMainWeapon()->GetStaminaCost(AttackTypeTag);

	/** 1. 현재 상태가 공격 액션을 취할 수 있는 상태인가?
	 *  2. 애님 몽타주를 통해 무기를 손에 쥔 상태인지?
	 *  3. 공격을 할 최소한의 스태미나가 있는지?
	 */
	return StateComponent->IsCurrentStateEqualToAny(CheckTags) == false &&
			CombatComponent->IsCombatEnabled() == true &&
			AttributeComponent->CheckHasEnoughStamina(StaminaCost) == true;
}

FGameplayTag ASoul_Character::GetAttackPerform() const
{
	if (IsSprinting())
	{
		return Soul_GameplayTag::Character_Attack_Running;
	}
	return Soul_GameplayTag::Character_Attack_Light;
}

void ASoul_Character::Attack()
{
	// 스프린트 상태인지 아닌지 확인하고 그에 맞는 태그 반환
	const FGameplayTag AttackTypeTag = GetAttackPerform();

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ASoul_Character::SpecialAttack()
{
	const FGameplayTag AttackTypeTag = Soul_GameplayTag::Character_Attack_Special;
	
	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ASoul_Character::HeavyAttack()
{
	AutoToggleCombat();

	const FGameplayTag AttackTypeTag = Soul_GameplayTag::Character_Attack_Heavy;

	if (CanPerformAttack(AttackTypeTag))
	{
		ExecuteComboAttack(AttackTypeTag);
	}
}

void ASoul_Character::ExecuteComboAttack(const FGameplayTag& AttackTypeTag)
{
	if (StateComponent->GetCurrentState() != Soul_GameplayTag::Character_State_Attacking)
	{
		// 애니메이션은 끝났지만 아직 콤보 시퀀스가 유효할 때 : 추가 입력 기회를 준다.
		if (bComboSequenceRunning == true && bCanComboInput == false)
		{
			++ComboCounter;
			UE_LOG(LogTemp, Warning, TEXT("Additional input : Combo counter = %d"), ComboCounter);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT(">>> ComboSequence Started <<<"));
			ResetCombo();
			bComboSequenceRunning = true;
		}

		DoAttack(AttackTypeTag);

		// 콤보 종료 타이머 취소
		GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
	}
	// 콤보 윈도우가 열려 있을 때 : 최적의 타이밍
	else if (bCanComboInput)
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Hit!!!!"));
		bSavedComboInput = true;
	}
}

void ASoul_Character::DoAttack(const FGameplayTag& AttackTypeTag)
{
	check(StateComponent);
	check(AttributeComponent);
	check(CombatComponent);

	// 공격 필수 조건 : 무기를 소지하고 있을 때
	if (const ASoul_Weapon* Weapon = CombatComponent->GetMainWeapon())
	{
		// 공격 중 상태로 변경
		StateComponent->SetState(Soul_GameplayTag::Character_State_Attacking);
		StateComponent->ToggleMovementInput(false); // 이동 불가
		
		CombatComponent->SetLastAttackType(AttackTypeTag);

		AttributeComponent->ToggleStaminaRegeneration(false); // 스태미나 회복 타이머 취소

		UAnimMontage* Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		if (false == IsValid(Montage)) 
		{
			// 콤보 공격의 끝에 도달하면 0번 콤보 몽타주 세팅
			ComboCounter = 0;
			Montage = Weapon->GetMontageForTag(AttackTypeTag, ComboCounter);
		}

		PlayAnimMontage(Montage);

		const float StaminaCost = Weapon->GetStaminaCost(AttackTypeTag);
		
		AttributeComponent->DecreaseStamina(StaminaCost); // 스태미나 차감, CanPerformAttack 함수에서 최소 스태미나 검사 후 해당 함수 진입
		AttributeComponent->ToggleStaminaRegeneration(true, 1.5f); // 다시 공격하지 않으면 1.5초 뒤 스태미나 회복
	}
}

void ASoul_Character::ResetCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("Combo Reset"));

	bComboSequenceRunning = false;
	bCanComboInput = false;
	bSavedComboInput = false;
	ComboCounter = 0;
}

void ASoul_Character::EnableComboWindow()
{
	bCanComboInput = true;
	UE_LOG(LogTemp, Warning, TEXT("Combo Window Opened : Combo Counter : %d"), ComboCounter);
}

void ASoul_Character::DisableComboWindow()
{
	check(CombatComponent);

	bCanComboInput = false;
	if (bSavedComboInput)
	{
		bSavedComboInput = false;
		++ComboCounter;
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed : Advancing to next combo = %d"), ComboCounter);
		DoAttack(CombatComponent->GetLastAttackType());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Combo Window Closed : No input received"));
	}
}

void ASoul_Character::AttackFinished(const float ComboResetDelay)
{
	UE_LOG(LogTemp, Warning, TEXT("AttackFinished"));
	if (StateComponent)
	{
		StateComponent->ToggleMovementInput(true); // 여기서 State가 Clear 된다.
	}

	// ComboResetDelay 간 추가 콤보 입력 시간을 준 뒤 콤보 시퀀스 종료
	GetWorld()->GetTimerManager().SetTimer(ComboResetTimerHandle, this, &ThisClass::ResetCombo, ComboResetDelay, false);
}


