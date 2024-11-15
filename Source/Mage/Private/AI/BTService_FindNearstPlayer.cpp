// Copyright AFstz.


#include "AI/BTService_FindNearstPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearstPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();

	const FName TargetTag = OwningPawn->ActorHasTag(FName("Enemy")) ? FName("Player") : FName("Enemy");

	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, TargetActors);

	float DistanceToNearestTarget = TNumericLimits<float>::Max();
	AActor* NearestTarget = nullptr;
	for (AActor* Target : TargetActors) // 查找最近距离玩家
	{
		if (IsValid(OwningPawn) && IsValid(Target)) // 安全检查
		{
			const float DistanceToTarget = OwningPawn->GetDistanceTo(Target);
			if (DistanceToTarget < DistanceToNearestTarget)
			{
				DistanceToNearestTarget = DistanceToTarget;
				NearestTarget = Target;
			}
		}
	}
	// 设置行为树的黑板关键帧数据
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, FollowTargetSelector, NearestTarget);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, DistanceToNearestTarget);
}