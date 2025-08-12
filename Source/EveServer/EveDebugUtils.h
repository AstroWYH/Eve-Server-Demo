#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EveDebugUtils.generated.h"

UCLASS()
class EVESERVER_API UEveDebugUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 封装的日志打印函数，带前缀
	 * @param Key 日志唯一标识（-1表示不覆盖）
	 * @param TimeToDisplay 显示时间（秒）
	 * @param DisplayColor 显示颜色
	 * @param DebugMessage 日志内容
	 * @param DebugPrefix 日志前缀（如[Server]、[Client]）
	 */
	UFUNCTION(BlueprintCallable, Category = "Eve|Debug")
	static void Log(int32 Key, float TimeToDisplay, FColor DisplayColor, const FString& DebugMessage);
	
};