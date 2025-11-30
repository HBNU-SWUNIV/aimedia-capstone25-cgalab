// MapGraphComponent.cpp
#include "MapGraphComponent.h"
#include "MapNodeActor.h"
#include "EngineUtils.h"
#include "Algo/RandomShuffle.h"
#include "Containers/Queue.h"

UMapGraphComponent::UMapGraphComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMapGraphComponent::CollectNodesInLevel()
{
	Nodes.Reset();
	UWorld* World = GetWorld();
	if (!World) return;

	for (TActorIterator<AMapNodeActor> It(World); It; ++It)
	{
		Nodes.Add(*It);
	}
}

AMapNodeActor* UMapGraphComponent::FindNodeByID(int32 NodeID) const
{
	for (AMapNodeActor* Node : Nodes)
	{
		if (IsValid(Node) && Node->NodeID == NodeID)
		{
			return Node;
		}
	}
	return nullptr;
}

bool UMapGraphComponent::PathExists(int32 StartNodeID, int32 EndNodeID) const
{
	const AMapNodeActor* Start = FindNodeByID(StartNodeID);
	const AMapNodeActor* End = FindNodeByID(EndNodeID);
	if (!Start || !End || Start->bIsBlocked || End->bIsBlocked)
	{
		return false;
	}

	// BFS
	TSet<int32> Visited;
	TQueue<const AMapNodeActor*> Q;
	Visited.Add(Start->NodeID);
	Q.Enqueue(Start);

	while (!Q.IsEmpty())
	{
		const AMapNodeActor* Cur = nullptr;
		Q.Dequeue(Cur);
		if (!Cur) continue;
		if (Cur->NodeID == EndNodeID)
		{
			return true;
		}

		for (AMapNodeActor* Next : Cur->ConnectedNodes)
		{
			if (!IsValid(Next) || Next->bIsBlocked)
			{
				continue;
			}
			if (!Visited.Contains(Next->NodeID))
			{
				Visited.Add(Next->NodeID);
				Q.Enqueue(Next);
			}
		}
	}

	return false;
}

int32 UMapGraphComponent::RandomlyBlockNode(int32 StartNodeID, int32 EndNodeID, int32 nBlocked)
{
	if (Nodes.Num() == 0)
	{
		CollectNodesInLevel();
	}

	// Build candidate list (exclude start and end, and already blocked)
	TArray<AMapNodeActor*> Candidates;
	Candidates.Reserve(Nodes.Num());
	for (AMapNodeActor* Node : Nodes)
	{
		if (!IsValid(Node)) continue;
		if (Node->NodeID == StartNodeID || Node->NodeID == EndNodeID) continue;
		if (Node->bIsBlocked) continue;
		Candidates.Add(Node);
	}

	// Shuffle for randomness
	Algo::RandomShuffle(Candidates);

	int32 ActuallyBlocked = 0;
	for (AMapNodeActor* Node : Candidates)
	{
		if (ActuallyBlocked >= nBlocked)
		{
			break;
		}

		// Tentatively block
		Node->SetBlocked(true);

		// Validate path
		if (PathExists(StartNodeID, EndNodeID))
		{
			ActuallyBlocked++;
		}
		else
		{
			// Revert if path broken
			Node->SetBlocked(false);
		}
	}

	// Update edge visuals across nodes once
	DrawAllEdgesOnce();
	return ActuallyBlocked;
}

void UMapGraphComponent::UnblockAll()
{
	for (AMapNodeActor* Node : Nodes)
	{
		if (IsValid(Node))
		{
			Node->SetBlocked(false);
		}
	}
	DrawAllEdgesOnce();
}

void UMapGraphComponent::DrawAllEdgesOnce()
{
	for (AMapNodeActor* Node : Nodes)
	{
		if (IsValid(Node))
		{
			Node->DrawEdgesOnce();
		}
	}
}
