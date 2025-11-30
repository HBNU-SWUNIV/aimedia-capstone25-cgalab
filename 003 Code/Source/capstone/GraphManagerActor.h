// GraphManagerActor.h - Simple actor to host the MapGraphComponent and expose single-call Blueprint API
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphManagerActor.generated.h"

class UMapGraphComponent;

UCLASS()
class CAPSTONE_API AGraphManagerActor : public AActor
{
	GENERATED_BODY()

public:
	AGraphManagerActor();

	virtual void BeginPlay() override;

	// The graph component managing nodes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Graph")
	TObjectPtr<UMapGraphComponent> Graph;

	// Wrapper to call from Blueprint
	UFUNCTION(BlueprintCallable, Category="Graph")
	int32 RandomlyBlockNode(int32 StartNodeID, int32 EndNodeID, int32 nBlocked);

	UFUNCTION(BlueprintCallable, Category="Graph")
	bool PathExists(int32 StartNodeID, int32 EndNodeID) const;

	UFUNCTION(BlueprintCallable, Category="Graph")
	void UnblockAll();
};
