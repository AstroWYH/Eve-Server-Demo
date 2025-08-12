// Fill out your copyright notice in the Description page of Project Settings.

#include "EveGameStateBase.h"

#include "EveDebugUtils.h"

void AEveGameStateBase::Multicast_BroadcastChat_Implementation(const FString& From, const FString& Message)
{
	if (HasAuthority())
	{
		return;
	}

	const FString Full = FString::Printf(TEXT("[Multicast] [%s]: %s"), *From, *Message);
	
	UEveDebugUtils::Log(-1, 30.f, FColor::Cyan, Full);
}
