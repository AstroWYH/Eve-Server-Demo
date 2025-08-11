#pragma once
#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "EvePlayerController.generated.h"

class AEvePickupActor;

UCLASS()
class AEvePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;

	// 本地（客户端）按键处理 -> 发起 Server RPC
	UFUNCTION()
	void LocalInteract();

	// 客户端 -> 服务器：请求拾取（放在 PlayerController 上，client 所属 actor）
	UFUNCTION(Server, Reliable)
	void Server_TryPickup(AEvePickupActor* Pickup);

	// 服务器 -> 指定客户端：通知（例如：你的分数变了）
	UFUNCTION(Client, Reliable)
	void Client_NotifyPickup(int32 NewScore);

	// 聊天：客户端 -> 服务器（服务器会广播）
	UFUNCTION(Server, Reliable)
	void Server_SendChatMessage(const FString& Msg);

	// 快速测试：控制台命令调用（在本地把消息发送到 Server）
	UFUNCTION(Exec)
	void Chat(const FString& Msg);
};