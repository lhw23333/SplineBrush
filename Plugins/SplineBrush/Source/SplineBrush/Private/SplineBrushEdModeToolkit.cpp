// Copyright Epic Games, Inc. All Rights Reserved.

#include "SplineBrushEdModeToolkit.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyEditorModule.h"
#include "SplineBrushEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "EditorModeManager.h"
#include "SSplinePalette.h"



#define LOCTEXT_NAMESPACE "FSplineBrushEdModeToolkit"

FSplineBrushEdModeToolkit::FSplineBrushEdModeToolkit()
{
}

void FSplineBrushEdModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	struct Locals
	{
		static bool IsWidgetEnabled()
		{
			return GEditor->GetSelectedActors()->Num() != 0;
		}

		
		
		static FReply OnButtonClick(FVector InOffset)
		{
			
			USelection* SelectedActors = GEditor->GetSelectedActors();
	
			// Let editor know that we're about to do something that we want to undo/redo
			GEditor->BeginTransaction(LOCTEXT("MoveActorsTransactionName", "MoveActors"));

			// For each selected actor
			for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
			{
				if (AActor* LevelActor = Cast<AActor>(*Iter))
				{
					// Register actor in opened transaction (undo/redo)
					LevelActor->Modify();
					// Move actor to given location
					LevelActor->TeleportTo(LevelActor->GetActorLocation() + InOffset, FRotator(0, 0, 0));
				}
			}

			// We're done moving actors so close transaction
			GEditor->EndTransaction();

			return FReply::Handled();
		}

		static TSharedRef<SWidget> MakeButton(FText InLabel, const FVector InOffset)
		{
			return SNew(SButton)
				.Text(InLabel)
				.OnClicked_Static(&Locals::OnButtonClick, InOffset);
		}

		
	};
	
	SAssignNew(ToolkitWidget, SVerticalBox)
	+ SVerticalBox::Slot()
	.AutoHeight()
	.HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSplinePalette)
		]
	];
		
			/*SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(50)
			[
				SNew(STextBlock)
				.AutoWrapText(true)
				.Text(LOCTEXT("HelperLabel", "请开始绘制spline"))
			]*/
			/*PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
				FAssetData(),
				false,
				ClassFilters,
				PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(ClassFilters),
				FOnShouldFilterAsset(),
				FOnAssetSelected::CreateSP(this,&)//set 设置
				OnClose)*/
			
			

		
	FModeToolkit::Init(InitToolkitHost);
}




FName FSplineBrushEdModeToolkit::GetToolkitFName() const
{
	return FName("SplineBrushEdMode");
}

FText FSplineBrushEdModeToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("SplineBrushEdModeToolkit", "DisplayName", "SplineBrushEdMode Tool");
}

class FEdMode* FSplineBrushEdModeToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);
}

#undef LOCTEXT_NAMESPACE
