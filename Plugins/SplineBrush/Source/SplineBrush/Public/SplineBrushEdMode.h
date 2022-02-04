// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <stack>
#include "CoreMinimal.h"
#include "ContentBrowserModule.h"
#include "EdMode.h"
#include "MyActor.h"
#include "SComponentClassCombo.h"

enum class ESplineBrushEditingState : uint8
{
	Unknown,
	Enabled,
	PIEWorld,
	SIEWorld,
};

struct BrushActorInfo
{
	FVector Location;

	UStaticMesh* Mesh;

	BrushActorInfo(FVector InLocation,UStaticMesh *InMesh):
	Location(InLocation),
	Mesh(InMesh)
	{
		
	}
};

class FSplineBrushEdMode : public FEdMode
{
public:
	const static FEditorModeID EM_SplineBrushEdModeId;
public:
	FSplineBrushEdMode();
	virtual ~FSplineBrushEdMode();

	// FEdMode interface
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	//virtual void Render(const FSceneView* View, FViewport* Viewport, FPrimitiveDrawInterface* PDI) override;
	//virtual void ActorSelectionChangeNotify() override;
	bool UsesToolkits() const override;
	// End of FEdMode interface

	//assest menu need
public:
	FText BrushActorName;
	
	UStaticMesh* BrushActor = nullptr;

	FVector BrushLocation;
	
	TArray<const UClass*> DisallowedClasses;
	
	TArray<UFactory*> NewAssetFactories;

	FOnShouldFilterAsset OnShouldFilterAsset;

	FOnAssetSelected OnSet;
	FSimpleDelegate OnClose;

	TSharedPtr<IPropertyHandle> PropertyHandle;

	TArray<FAssetData> OwnerAssetArray;

	TArray<const UClass*> ClassFilters;
	//ClassFilters.Add(AActor::StaticClass());

	std::stack<AMyActor*> CurrentStack;
	std::stack<BrushActorInfo> RemoveStack;
	
	//SplineBrushTrace 是否开启
	bool bToolActive;
	//bool bCanAltDrag;
	//bool bAdjustBrushRadius;
	//笔刷的设置
	
	bool bBrushTraceValid;
	FVector BrushNormal;
	FVector BrushTraceDirection;
	UStaticMeshComponent* SphereBrushComponent;
	/** The dynamic material of the sphere brush. */
	class UMaterialInstanceDynamic* BrushMID;
	FColor BrushDefaultHighlightColor;
	FColor BrushCurrentHighlightColor;
	/** Default opacity received from the brush material to reset it when closing. */
	float DefaultBrushOpacity;

	ESplineBrushEditingState GetEditingState() const;
	bool IsEditingEnabled() const
	{
		return GetEditingState() == ESplineBrushEditingState::Enabled;
	}

	FText GetBrushActorName()
	{
		return BrushActorName;
	}

	//UActorComponent* PerformComboAddClass(TSubclassOf<UActorComponent> ComponentClass, EComponentCreateAction::Type ComponentCreateAction, UObject* AssetOverride);

	//UClass* CreateNewCPPComponent( TSubclassOf<UActorComponent> ComponentClass );
	
	virtual  bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	
	//virtual  bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	
	/** Start foliage tracing */
	void StartSplineBrushTrace(FEditorViewportClient* ViewportClient, class UViewportInteractor* Interactor = nullptr);

	/* End foliage tracing*/
	void EndSplineBrushTrace();

	/** Trace under the mouse cursor and update brush position */
	void SplineBrushTrace(FEditorViewportClient* ViewportClient, const FVector& InRayOrigin, const FVector& InRayDirection);
};
