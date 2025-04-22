#include "PowerLines.h"

#include "CableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

APowerLines::APowerLines()
{
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
}

void APowerLines::UpdateWires()
{
	ClearWires();
	if (SocketTargetTransforms.IsEmpty())
	{
		return;
	}
	TArray<FName> SocketNames = StaticMesh->GetAllSocketNames();
	for (int i = 0; i < SocketNames.Num(); i++)
	{
		FTransform TargetTransform = StaticMesh->GetSocketTransform(SocketNames[i], ERelativeTransformSpace::RTS_Actor);
		UCableComponent* NewCable = Cast<UCableComponent>(
			AddComponentByClass(UCableComponent::StaticClass(), false, TargetTransform, false));
		if (!IsValid(NewCable))
		{
			return;
		}
		Cables.Add(NewCable);
		NewCable->EndLocation = UKismetMathLibrary::InverseTransformLocation(
			GetActorTransform(), SocketTargetTransforms[i]);
		NewCable->CableWidth = CableWidth;
		NewCable->CableGravityScale = CableGravityScale;
	}
}

void APowerLines::ClearWires()
{
	for (auto Cable : Cables)
	{
		if (IsValid(Cable))
		{
			Cable->DestroyComponent();
		}
	}
	Cables.Empty();
}

void APowerLines::UpdateSocketLocations()
{
	APowerLines* Next = Cast<APowerLines>(NextPoint.Get());
	if (!IsValid(Next))
	{
		return;
	}
	SocketTargetTransforms.Empty();
	TArray<FName> SocketNames = Next->StaticMesh->GetAllSocketNames();
	for (auto SocketName : SocketNames)
	{
		SocketTargetTransforms.Add(Next->StaticMesh->GetSocketLocation(SocketName));
	}
}

void APowerLines::BeginPlay()
{
	Super::BeginPlay();
	UpdateWires();
}

void APowerLines::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ClearWires();
}

void APowerLines::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	APowerLines* Next = Cast<APowerLines>(NextPoint.Get());
	if (!IsValid(Next))
	{
		return;
	}
	Next->PreviousPoint = this;
	APowerLines* Previous = Cast<APowerLines>(PreviousPoint.Get());
	if (!IsValid(Previous))
	{
		return;
	}
	Previous->UpdateSocketLocations();
}
