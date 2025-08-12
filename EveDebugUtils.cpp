#include "EveDebugUtils.h"
#include "Engine/Engine.h"
DECLARE_LOG_CATEGORY_CLASS(EveLog, Display, All);

void UEveDebugUtils::Log(int32 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage)
{
	if (GEngine)
	{
		FString FullMessage = DebugMessage;
		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, DisplayColor, FullMessage);
		UE_LOG(EveLog, Log, TEXT("%s"), *FullMessage); // 同时输出到控制台日志
	}
}
