#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "EvePlayerState.generated.h"

UCLASS()
class AEvePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AEvePlayerState();

	// 这是要同步给所有客户端的分数
	UPROPERTY(ReplicatedUsing = OnRep_Point)
	int32 Point;

	UFUNCTION()
	void OnRep_Point();

	// 服务器端调用以增加分数
	void AddScore(int32 Delta);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE int32 GetScoreValue() const { return Point; }
};
