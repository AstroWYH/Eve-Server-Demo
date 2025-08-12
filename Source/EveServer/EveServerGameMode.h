// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EveServerGameMode.generated.h"

UCLASS(minimalapi)
class AEveServerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AEveServerGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};