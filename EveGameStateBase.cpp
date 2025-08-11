// Fill out your copyright notice in the Description page of Project Settings.

#include "EveGameStateBase.h"

void AEveGameStateBase::Multicast_BroadcastChat_Implementation(const FString& From, const FString& Message)
{
	if (HasAuthority())
	{
		return;
	}

	const FString Full = FString::Printf(TEXT("[Multicast] [%s]: %s"), *From, *Message);
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Cyan, Full);
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Full);
}