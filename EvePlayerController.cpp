#include "EvePlayerController.h"
#include "EvePickupActor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "EveDebugUtils.h"
#include "Kismet/GameplayStatics.h"
#include "EveGameStateBase.h"
#include "EvePlayerState.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void AEvePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// 需要在 Project Settings -> Input 添加 Action Mapping 名为 "Interact"（按 E）
	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &AEvePlayerController::LocalInteract);
	}
}

void AEvePlayerController::LocalInteract()
{
	if (!IsLocalController()) return; // 只在本地客户端检测输入并发请求

	FVector Start;
	FRotator Rot;
	GetPlayerViewPoint(Start, Rot);
	FVector End = Start + Rot.Vector() * 500.f;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetPawn());
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.f, 0, 1.f);

	if (bHit)
	{
		AEvePickupActor* Pickup = Cast<AEvePickupActor>(Hit.GetActor());
		if (Pickup)
		{
			// 把拾取请求发到服务器（因为 PlayerController 是 owned，RPC 会被转发）
			Server_TryPickup(Pickup);
		}
	}
}

void AEvePlayerController::Server_TryPickup_Implementation(AEvePickupActor* Pickup)
{
	if (!HasAuthority()) return;
	if (!Pickup) return;
	
	UEveDebugUtils::Log(-1, 30.f, FColor::Blue, FString::Printf(TEXT("[RPC server] %s: pick up request"), *PlayerState->GetPlayerName()));

	APawn* EvePawn = GetPawn();
	if (!EvePawn) return;

	// 服务器端验证（防作弊）：距离、是否存在等
	const float MaxDist = 300.f;
	if (FVector::DistSquared(EvePawn->GetActorLocation(), Pickup->GetActorLocation()) > FMath::Square(MaxDist))
	{
		// 距离太远，忽略
		return;
	}

	// 让拾取物在服务器上执行 HandlePickup（会修改 PlayerState、销毁 Actor、Multicast 效果）
	Pickup->HandlePickup(this);

	// 另外，直接通知这个客户端（示范 Client RPC），也可以不做（因为 PlayerState 会 Replicate）
	AEvePlayerState* PS = GetPlayerState<AEvePlayerState>();
	if (PS)
	{
		Client_NotifyPickup(PS->GetScoreValue());
	}
}

void AEvePlayerController::Client_NotifyPickup_Implementation(int32 NewScore)
{
	if (GEngine)
	{
		// 判断当前角色（Listen Server 或 Client）
		FString RoleTag;
		if (GetWorld() && GetWorld()->GetAuthGameMode() && GetWorld()->GetFirstPlayerController() && GetWorld()->GetFirstPlayerController()->IsLocalController())
		{
			RoleTag = TEXT("[ListenSrv]"); // Listen Server 本地客户端
		}
		else
		{
			RoleTag = TEXT("[Client]"); // 纯客户端
		}

		FString LogStr = FString::Printf(TEXT("%s [RPC client] %s: score updated: %d"), 
			*RoleTag, *PlayerState->GetPlayerName(), NewScore);
		UEveDebugUtils::Log(-1, 30.f, FColor::Blue, LogStr);
	}
}

void AEvePlayerController::Server_SendChatMessage_Implementation(const FString& Msg)
{
	if (!HasAuthority()) return;

	AEveGameStateBase* GS = GetWorld() ? GetWorld()->GetGameState<AEveGameStateBase>() : nullptr;
	FString Sender = PlayerState ? PlayerState->GetPlayerName() : TEXT("Unknown");

	UEveDebugUtils::Log(-1, 30.f, FColor::Blue, FString::Printf(TEXT("[RPC server] %s: send chat msg request"), *PlayerState->GetPlayerName()));
	
	if (GS)
	{
		GS->Multicast_BroadcastChat(Sender, Msg);
	}
}

void AEvePlayerController::Chat(const FString& Msg)
{
	// console exec (local) -> send to server
	if (IsLocalController())
	{
		Server_SendChatMessage(Msg);
	}
}