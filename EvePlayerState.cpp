#include "EvePlayerState.h"

#include "EveDebugUtils.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

AEvePlayerState::AEvePlayerState()
{
	Point = 0;
	bReplicates = true;
}

void AEvePlayerState::OnRep_Point()
{
	// 在客户端执行：更新 HUD / 显示提示
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

		FString LogStr = FString::Printf(TEXT("%s [Client OnRep] %s score updated: %d"), *RoleTag, *GetPlayerName(), Point);
		UEveDebugUtils::Log(-1, 30.f, FColor::Green, LogStr);
	}
}

void AEvePlayerState::AddScore(int32 Delta)
{
	if (HasAuthority())
	{
		Point += Delta;
		// server 端可在这里做额外逻辑
	}
}

void AEvePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AEvePlayerState, Point);
}