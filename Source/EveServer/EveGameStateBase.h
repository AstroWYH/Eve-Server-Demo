// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "EveGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class EVESERVER_API AEveGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	// server 调用 -> 在所有客户端（包括服务器本地）执行
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_BroadcastChat(const FString& From, const FString& Message);
};