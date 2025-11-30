// MapNodeActor.h - Represents a node (area) in the escape map graph using a Billboard
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapNodeActor.generated.h"

class UBillboardComponent;

UCLASS(BlueprintType, Blueprintable)
class CAPSTONE_API AMapNodeActor : public AActor
{
	GENERATED_BODY()

public:
	AMapNodeActor();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Tick(float DeltaSeconds) override;
#if WITH_EDITOR
	// Allow ticking in editor viewport so debug lines update without leaving trails
	virtual bool ShouldTickIfViewportsOnly() const override { return true; }
#endif

	// Unique node id set by designer in editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Graph")
	int32 NodeID = -1;

	// Connected node references set by designer in editor (undirected graph)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Graph")
	TArray<TObjectPtr<AMapNodeActor>> ConnectedNodes;

	// Blocked state. When blocked, node is not traversable.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Graph")
	bool bIsBlocked = false;

	// Billboard visualization (uses default engine texture if none assigned)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visual")
	TObjectPtr<UBillboardComponent> Billboard;

	// Edge debug draw settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual|Edges")
	bool bDrawEdgesEveryTick = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual|Edges")
	FColor EdgeColor = FColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Visual|Edges")
	float EdgeThickness = 3.f;

	// Toggle blocked state and update visuals
	UFUNCTION(BlueprintCallable, Category="Graph")
	void SetBlocked(bool bBlocked);

	// Explicit refresh (sprite visibility and edges once)
	UFUNCTION(BlueprintCallable, Category="Graph")
	void RefreshVisuals();

	// Draw edges once (debug)
	UFUNCTION(BlueprintCallable, Category="Graph")
	void DrawEdgesOnce();

private:
	void DrawEdgesInternal(bool bPersistent);
};
