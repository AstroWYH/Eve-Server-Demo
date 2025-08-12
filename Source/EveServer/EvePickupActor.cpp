#include "EvePickupActor.h"

#include "EveDebugUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "EvePlayerState.h"
#include "Engine/Engine.h"

AEvePickupActor::AEvePickupActor()
{
	bReplicates = true;
	SetReplicateMovement(true);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MeshComp->SetIsReplicated(true);
}

void AEvePickupActor::HandlePickup(AController* InstigatingController)
{
	if (!HasAuthority()) return;
	if (!InstigatingController) return;

	// 给玩家加分（服务器端做分数修改）
	AEvePlayerState* PS = InstigatingController->GetPlayerState<AEvePlayerState>();
	if (PS)
	{
		PS->AddScore(ScoreValue);
	}

	// 通知所有客户端播放效果
	Multicast_PlayPickupEffects();

	// 服务端销毁（会同步到客户端）
	Destroy();
}

void AEvePickupActor::Multicast_PlayPickupEffects_Implementation()
{
	// 在这里可以播放粒子、声音、震屏等（示例用日志和屏幕提示）
	UEveDebugUtils::Log(-1, 30.f, FColor::Yellow, TEXT("[Multicast] Pickup: effect (multicast)"));

	// 例如：
	// UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

void AEvePickupActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// ScoreValue 不需要复制（设计常量），如果你希望它可变并同步到客户端，可以 DOREPLIFETIME 它
}
