#include "PowerLines.h"

#include "CableComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

APowerLines::APowerLines()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
}

void APowerLines::UpdateWires()
{
	ClearWires();
	if (SocketTargetTransforms.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("There are no target transforms: %s"), *GetName());
		return;
	}
	TArray<FName> SocketNames = StaticMesh->GetAllSocketNames();
	for (int32 Index = 0; Index < SocketNames.Num(); Index++)
	{
		FTransform TargetTransform = StaticMesh->GetSocketTransform(SocketNames[Index], ERelativeTransformSpace::RTS_Actor);
		UCableComponent* NewCable = Cast<UCableComponent>(
			AddComponentByClass(UCableComponent::StaticClass(), false, TargetTransform, false));
		if (!IsValid(NewCable))
		{
			UE_LOG(LogTemp, Log, TEXT("Failed to create new cable"));
			continue;
		}
		Cables.Add(NewCable);
		NewCable->EndLocation = UKismetMathLibrary::InverseTransformLocation(
			GetActorTransform(), SocketTargetTransforms[Index]);
		NewCable->CableWidth = CableWidth;
		NewCable->CableGravityScale = CableGravityScale;
	}
}

void APowerLines::ClearWires()
{
	for (UCableComponent* Cable : Cables)
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
	SocketTargetTransforms.Reset();
	if(!NextPoint.IsValid())
	{
		return;
	}
	APowerLines* Next = Cast<APowerLines>(NextPoint.Get());
	if (!IsValid(Next) && !IsValid(Next->StaticMesh))
	{
		UE_LOG(LogTemp, Log, TEXT("%s Invalid setup of next point"), *GetName());
		return;
	}
	TArray<FName> SocketNames = Next->StaticMesh->GetAllSocketNames();
	for (FName SocketName : SocketNames)
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
	ClearWires();
	Super::EndPlay(EndPlayReason);
}

void APowerLines::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if(!NextPoint.IsValid())
	{
		return;
	}
	APowerLines* Next = Cast<APowerLines>(NextPoint.Get());
	if (IsValid(Next))
	{
		Next->PreviousPoint = this;
	}
	if(!PreviousPoint.IsValid())
	{
		return;
	}
	APowerLines* Previous = Cast<APowerLines>(PreviousPoint.Get());
	if (IsValid(Previous))
	{
		Previous->UpdateSocketLocations();
	}
	
}
