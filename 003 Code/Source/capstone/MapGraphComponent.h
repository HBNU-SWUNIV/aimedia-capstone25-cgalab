// MapGraphComponent.h - Manages the whole graph and blocking logic (simplified)
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapGraphComponent.generated.h"

class AMapNodeActor;

UCLASS(ClassGroup=(Custom), BlueprintType, meta=(BlueprintSpawnableComponent))
class CAPSTONE_API UMapGraphComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMapGraphComponent();

	UFUNCTION(BlueprintCallable, Category="Graph")
	void CollectNodesInLevel();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Graph")
	TArray<TObjectPtr<AMapNodeActor>> Nodes;

	UFUNCTION(BlueprintCallable, Category="Graph")
	bool PathExists(int32 StartNodeID, int32 EndNodeID) const;

	UFUNCTION(BlueprintCallable, Category="Graph")
	int32 RandomlyBlockNode(int32 StartNodeID, int32 EndNodeID, int32 nBlocked);

	UFUNCTION(BlueprintCallable, Category="Graph")
	void UnblockAll();

	UFUNCTION(BlueprintCallable, Category="Graph|Debug")
	void DrawAllEdgesOnce();

private:
	AMapNodeActor* FindNodeByID(int32 NodeID) const;
};
