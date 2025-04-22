#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerLines.generated.h"


class UStaticMeshComponent;
class UCableComponent;

UCLASS()
class UETOOLS_API APowerLines : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Lines")
	TSoftObjectPtr<APowerLines> NextPoint = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Power Lines")
	TSoftObjectPtr<APowerLines> PreviousPoint = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Lines")
	float CableWidth = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power Lines")
	float CableGravityScale = 1.0f;

	UPROPERTY(VisibleAnywhere, Category = "Power Lines")
	TArray<TObjectPtr<UCableComponent>> Cables = {};
	UPROPERTY(VisibleAnywhere, Category = "Power Lines")
	TArray<FVector> SocketTargetTransforms = {};
	
public:
	APowerLines();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Power Lines")
	void UpdateWires();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Power Lines")
	void ClearWires();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Power Lines")
	void UpdateSocketLocations();
};
