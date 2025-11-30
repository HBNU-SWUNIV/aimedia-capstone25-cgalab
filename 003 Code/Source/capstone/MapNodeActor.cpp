// MapNodeActor.cpp - Billboard-based node actor with clean editor drawing
#include "MapNodeActor.h"
#include "Components/BillboardComponent.h"
#include "DrawDebugHelpers.h"
#include "UObject/ConstructorHelpers.h"

AMapNodeActor::AMapNodeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	// Default sprite for convenience
	if (Billboard)
	{
		static ConstructorHelpers::FObjectFinderOptional<UTexture2D> DefaultSprite(TEXT("Texture2D'/Engine/EditorResources/S_Knot.S_Knot'"));
		if (DefaultSprite.Succeeded())
		{
			Billboard->SetSprite(DefaultSprite.Get());
		}
	}
}

void AMapNodeActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RefreshVisuals();
}

void AMapNodeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Draw non-persistent lines every frame so previous lines clear automatically (no trails)
	if (bDrawEdgesEveryTick)
	{
		DrawEdgesInternal(false /*bPersistent*/);
	}
}

void AMapNodeActor::SetBlocked(bool bBlocked)
{
	bIsBlocked = bBlocked;
	RefreshVisuals();
}

void AMapNodeActor::RefreshVisuals()
{
	if (Billboard)
	{
		Billboard->SetVisibility(!bIsBlocked, true);
	}
}

void AMapNodeActor::DrawEdgesOnce()
{
	DrawEdgesInternal(false);
}

void AMapNodeActor::DrawEdgesInternal(bool bPersistent)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector MyLoc = GetActorLocation();
	const FColor ColorToUse = bIsBlocked ? FColor::Red : FColor::Green;

	for (AMapNodeActor* Other : ConnectedNodes)
	{
		if (!IsValid(Other) || Other == this || Other->bIsBlocked) continue;
		DrawDebugLine(World, MyLoc, Other->GetActorLocation(), ColorToUse, bPersistent, 0.f, 0, EdgeThickness);
	}
}
