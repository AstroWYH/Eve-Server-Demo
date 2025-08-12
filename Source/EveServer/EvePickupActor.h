#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EvePickupActor.generated.h"

UCLASS()
class AEvePickupActor : public AActor
{
	GENERATED_BODY()
public:
	AEvePickupActor();

	// 交互获得的分数值（可在编辑器调整）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	int32 ScoreValue = 1;

	// 一个简单的静态网格（可在 BP/编辑器替换）
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	// 由服务器调用，处理拾取（验证在 PlayerController 的 Server RPC 中完成）
	void HandlePickup(AController* InstigatingController);

	// 在所有客户端上播放拾取效果（粒子 / 声音 / UI 辅助）
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayPickupEffects();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
