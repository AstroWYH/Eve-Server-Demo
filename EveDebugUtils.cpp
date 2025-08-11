#include "EveDebugUtils.h"
#include "Engine/Engine.h"

void UEveDebugUtils::Log(int32 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage, const FString& DebugPrefix)
{
	if (GEngine)
	{
		FString FullMessage = DebugPrefix + DebugMessage;
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, DisplayColor, FullMessage);
		UE_LOG(LogTemp, Log, TEXT("%s"), *FullMessage); // 同时输出到控制台日志
	}
}
