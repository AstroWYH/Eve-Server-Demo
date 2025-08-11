// Copyright Epic Games, Inc. All Rights Reserved.

#include "EveServerGameMode.h"

#include "EveGameStateBase.h"
#include "EvePlayerController.h"
#include "EvePlayerState.h"
#include "EveServerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AEveServerGameMode::AEveServerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AEveServerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	static int32 PlayerIndex = 0;
	TArray<FString> NamePool = {TEXT("小明"), TEXT("小红"), TEXT("小刚"), TEXT("小芳"), TEXT("小李"), TEXT("小张"), TEXT("小赵"), TEXT("小周")};

	FString NewName;
	if (PlayerIndex < NamePool.Num())
	{
		NewName = NamePool[PlayerIndex];
	}
	else
	{
		// 超过名单后自动生成
		NewName = FString::Printf(TEXT("玩家%d"), PlayerIndex + 1);
	}

	if (NewPlayer->PlayerState)
	{
		// 服务器改名，并同步到所有客户端
		NewPlayer->PlayerState->SetPlayerName(NewName);
	}

	PlayerIndex++;
}