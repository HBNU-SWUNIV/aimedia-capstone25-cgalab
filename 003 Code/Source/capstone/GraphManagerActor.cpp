// GraphManagerActor.cpp
#include "GraphManagerActor.h"
#include "MapGraphComponent.h"

AGraphManagerActor::AGraphManagerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Graph = CreateDefaultSubobject<UMapGraphComponent>(TEXT("MapGraph"));
}

void AGraphManagerActor::BeginPlay()
{
	Super::BeginPlay();
	if (Graph)
	{
		Graph->CollectNodesInLevel();
	}
}

int32 AGraphManagerActor::RandomlyBlockNode(int32 StartNodeID, int32 EndNodeID, int32 nBlocked)
{
	return Graph ? Graph->RandomlyBlockNode(StartNodeID, EndNodeID, nBlocked) : 0;
}

bool AGraphManagerActor::PathExists(int32 StartNodeID, int32 EndNodeID) const
{
	return Graph ? Graph->PathExists(StartNodeID, EndNodeID) : false;
}

void AGraphManagerActor::UnblockAll()
{
	if (Graph)
	{
		Graph->UnblockAll();
	}
}
