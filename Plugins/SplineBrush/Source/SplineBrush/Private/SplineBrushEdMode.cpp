// Copyright Epic Games, Inc. All Rights Reserved.

#include "SplineBrushEdMode.h"
#include "CollisionDebugDrawingPublic.h"
#include "DrawDebugHelpers.h"
#include "PropertyCustomizationHelpers.h"
#include "SplineBrushEdModeToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "GameProjectGenerationModule.h"
#include "StaticMeshAttributes.h"
#include "VREditorMode.h"
#include "Kismet/Public/SSCSEditor.h"
#include "MyActor.h"
#include "Math/Quat.h"
#include "Engine/Public/CollisionQueryParams.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



class ALandscapeProxy;
class ULandscapeHeightfieldCollisionComponent;
const FEditorModeID FSplineBrushEdMode::EM_SplineBrushEdModeId = TEXT("EM_SplineBrushEdMode");

FSplineBrushEdMode::FSplineBrushEdMode()
{

}

FSplineBrushEdMode::~FSplineBrushEdMode()
{

}

void FSplineBrushEdMode::Enter()
{
	FEdMode::Enter();

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		Toolkit = MakeShareable(new FSplineBrushEdModeToolkit);
		Toolkit->Init(Owner->GetToolkitHost());
	}

	//BrushClass = 
}

void FSplineBrushEdMode::Exit()
{
	if (Toolkit.IsValid())
	{
		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Call base Exit method to ensure proper cleanup
	FEdMode::Exit();
}

bool FSplineBrushEdMode::UsesToolkits() const
{
	return true;
}

ESplineBrushEditingState FSplineBrushEdMode::GetEditingState() const
{
	UWorld* World = GetWorld();

	if (GEditor->bIsSimulatingInEditor)
	{
		return ESplineBrushEditingState::SIEWorld;
	}
	else if (GEditor->PlayWorld != NULL)
	{
		return ESplineBrushEditingState::PIEWorld;
	}
	else if (World == nullptr)
	{
		return ESplineBrushEditingState::Unknown;
	}

	return ESplineBrushEditingState::Enabled;
}

bool FSplineBrushEdMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	if (!IsEditingEnabled())
	{
		return false;
	}

	if(bToolActive)
	{
		if (Key == EKeys::LeftMouseButton && Event == IE_Pressed  && IsCtrlDown(Viewport) && BrushActor != nullptr)
		{
			StartSplineBrushTrace(ViewportClient);
			
			UE_LOG(LogTemp,Warning,TEXT("leftbutton"));
			FVector Vector;
			FRotator Rotator;
			FActorSpawnParameters Spawn;
			
			AMyActor* BrushedActor = GetWorld()->SpawnActor<AMyActor>();
			BrushedActor->SetActorLocation(BrushLocation);
			BrushedActor->StaticMesh->SetStaticMesh(BrushActor);
			CurrentStack.push(BrushedActor);
	

			//清空恢复栈
			std::stack<BrushActorInfo>().swap(RemoveStack);
		
			
			//AMyActor* CloneActor =  GetWorld()->SpawnActor<AMyActor>();

			//CloneActor->SetActorLocation(CurrentStack.top()->GetActorLocation());
		}
		else if(Key == EKeys::Z && Event == IE_Pressed && IsCtrlDown(Viewport))
		{
			UE_LOG(LogTemp,Warning,TEXT("ctrl z"));

			if(CurrentStack.empty())
			{
				return false;
			}
			else
			{
				AMyActor* temp = CurrentStack.top();
				RemoveStack.push(BrushActorInfo(temp->GetActorLocation(),temp->StaticMesh->GetStaticMesh()));
				temp->Destroy();
				CurrentStack.pop();
			}
			
		}
		else if(Key== EKeys::Y && Event == IE_Pressed && IsCtrlDown(Viewport))
		{
			if(RemoveStack.empty())
			{
				return false;
			}
			else
			{
				AMyActor* Clone = GetWorld()->SpawnActor<AMyActor>();
				Clone->SetActorLocation(RemoveStack.top().Location);
				Clone->StaticMesh->SetStaticMesh(RemoveStack.top().Mesh);

				CurrentStack.push(Clone);
				RemoveStack.pop();
			}
		}
	}
	
	
	return false;
}

bool FSplineBrushEdMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 MouseX, int32 MouseY)
{

	//UVREditorMode* VREditorMode = Cast<UVREditorMode>( GEditor->GetEditorWorldExtensionsManager()->GetEditorWorldExtensions( GetWorld() )->FindExtension( UVREditorMode::StaticClass() ) );
	if (IsEditingEnabled())
	{
		// Compute a world space ray from the screen space mouse coordinates
		FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
			ViewportClient->Viewport,
			ViewportClient->GetScene(),
			ViewportClient->EngineShowFlags)
			.SetRealtimeUpdate(ViewportClient->IsRealtime()));

		FSceneView* View = ViewportClient->CalcSceneView(&ViewFamily);
		FViewportCursorLocation MouseViewportRay(View, ViewportClient, MouseX, MouseY);
		BrushTraceDirection = MouseViewportRay.GetDirection();

		FVector BrushTraceStart = MouseViewportRay.GetOrigin();
		FVector BrushTraceEnd = BrushTraceStart + BrushTraceDirection * HALF_WORLD_MAX;

		TArray<FHitResult> Hits;
		FCollisionQueryParams TraceParams(TEXT("LineTrace"), false);
		FCollisionShape SphereShape;
		FCollisionQueryParams QueryParams(FName("BrushParam"), SCENE_QUERY_STAT_ONLY(IFA_FoliageTrace), true);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByObjectType(Hit,BrushTraceStart,BrushTraceEnd,ECollisionChannel::ECC_WorldStatic);
		
		
		//GetWorld()->SweepMultiByObjectType(Hits,BrushTraceStart,BrushTraceEnd,FQuat::Identity,FCollisionObjectQueryParams(ECC_WorldStatic),SphereShape, QueryParams);
		//DrawLineTraces(GetWorld(),BrushTraceStart,BrushTraceEnd,Hits,1);
		UE_LOG(LogTemp,Warning,TEXT("%s"),*Hit.Location.ToString());
		DrawDebugSphere(GetWorld(),Hit.Location,50,10,FColor::Red);
		BrushLocation = Hit.Location;
		if (ViewportClient->IsOrtho())
		{
			BrushTraceStart += -WORLD_MAX * BrushTraceDirection;
		}

		
		
		//UE_LOG(LogTemp,Warning,TEXT("brush direction is %s"),*hit.Location.ToString());
		//UActorFactory* ActorFactory = GEditor->FindActorFactoryByClass(BrushClass);
		// if(ActorFactory)
		// {
		// 	UE_LOG(LogTemp,Warning,TEXT("actorFactory is %d"),*ActorFactory->GetName());
		// }
		
		
		SplineBrushTrace(ViewportClient, BrushTraceStart, BrushTraceDirection);
		
	}
	return false;
}


void FSplineBrushEdMode::StartSplineBrushTrace(FEditorViewportClient* ViewportClient, UViewportInteractor* Interactor)
{
	if(!bToolActive)
	{
		GEditor->BeginTransaction(NSLOCTEXT("UnrealEd", "FoliageMode_EditTransaction", "Foliage Editing"));




		bToolActive = true;
	}
}

void FSplineBrushEdMode::EndSplineBrushTrace()
{
	bToolActive = false;
}

void FSplineBrushEdMode::SplineBrushTrace(FEditorViewportClient* ViewportClient, const FVector& InRayOrigin, const FVector& InRayDirection)
{
	if (ViewportClient == nullptr || ( !ViewportClient->IsMovingCamera() && ViewportClient->IsVisible() ) )
	{
		const FVector TraceStart(InRayOrigin);
		const FVector TraceEnd(InRayOrigin + InRayDirection * HALF_WORLD_MAX);
		FHitResult Hit;
		UWorld* World = GetWorld();
		static FName NAME_FSplineBrush = FName(TEXT("SplineBrush"));
	}
}












