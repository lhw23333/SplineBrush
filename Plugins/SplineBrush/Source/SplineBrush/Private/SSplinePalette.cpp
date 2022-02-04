#include "SSplinePalette.h"


#include "Widgets/Layout/SSplitter.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Misc/MessageDialog.h"
#include "Misc/FeedbackContext.h"
#include "Modules/ModuleManager.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Text/SRichTextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Layout/SScrollBorder.h"
#include "Widgets/Input/SSlider.h"
#include "EditorStyleSet.h"
#include "AssetThumbnail.h"
#include "PropertyEditorModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "PropertyCustomizationHelpers.h"
#include "IDetailsView.h"
#include "AssetSelection.h"
#include "ScopedTransaction.h"
#include "AssetData.h"
#include "Engine/StaticMesh.h"
#include "Editor.h"
#include "Widgets/Layout/SScaleBox.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Input/SSearchBox.h"
#include "PropertyCustomizationHelpers.h"
#include "PropertyEditorModule.h"
#include "SplineBrushEdMode.h"
#include "Engine/Selection.h"
#include "Widgets/Input/SButton.h"
#include "EditorModeManager.h"
#include "SplineBrushEdModeToolkit.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SComboButton.h"
#include "Internationalization/Internationalization.h"
#include "EditorStyleSet.h"
#include "Components/SplineComponent.h"

#include <stack>
#include <ThirdParty/openexr/Deploy/OpenEXR-2.3.0/OpenEXR/include/ImathQuat.h>
#define LOCTEXT_NAMESPACE "BrushSplineEd_Mode"

void SSplinePalette::Construct(const FArguments& InArgs)
{
	
	BrushActorName.Bind(this,&SSplinePalette::GetBrushActorName);
	IsActive.Bind(this,&SSplinePalette::GetIsActive);
	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		[
			SAssignNew(SelectActorCombo,SComboButton)
			.ForegroundColor(FLinearColor::White)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
			.OnGetMenuContent(this, &SSplinePalette::GetActorPicker)
			.ContentPadding(FMargin(1.f))
			.ButtonContent()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				.Padding(1.f)
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "FoliageEditMode.AddFoliageType.Text")
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.9"))
					.Text(FText::FromString(FString(TEXT("\xf067"))) /*fa-plus*/)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.Padding(1.f)
				[
					//SNew(STextBlock)
					SAssignNew(Stext,STextBlock)
					.Text(BrushActorName)
					.TextStyle(FEditorStyle::Get(), "FoliageEditMode.AddFoliageType.Text")
				]

			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this,&SSplinePalette::OnCloseButtonClicked)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				.Padding(1.f)
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "FoliageEditMode.AddFoliageType.Text")
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.9"))
					.Text(FText::FromString(FString(TEXT("\xf067"))) /*fa-plus*/)
				]
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.Padding(1.f)
				[
					//SNew(STextBlock)
					SNew(STextBlock)
					.Text(IsActive)
					.TextStyle(FEditorStyle::Get(), "FoliageEditMode.AddFoliageType.Text")
				]
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this,&SSplinePalette::OnCreatSplineClicked)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				.Padding(1.f)
				[
					SNew(STextBlock)
					.TextStyle(FEditorStyle::Get(), "FoliageEditMode.AddFoliageType.Text")
					.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.9"))
					.Text(FText::FromString(FString(TEXT("\xf067"))) /*fa-plus*/)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.Padding(1.f)
				[
					//SNew(STextBlock)
					SNew(STextBlock)
					.Text(LOCTEXT("AddFoliageTypeButtonLabel", "生成Spline"))
					.TextStyle(FEditorStyle::Get(), "FoliageEditMode.AddFoliageType.Text")
				]
			]
		]
		+SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this,&SSplinePalette::OnCreatEndToEndSplineClicked)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.Padding(1.f)
				[
					//SNew(STextBlock)
					SNew(STextBlock)
					.Text(LOCTEXT("Create","生成Spline加入起始点"))
					.TextStyle(FEditorStyle::Get(), "FoliageEditMode.AddFoliageType.Text")
				]
			]
		]
	];
}




SSplinePalette::~SSplinePalette()
{
	
}

FReply SSplinePalette::OnCreatEndToEndSplineClicked()
{
	auto myMode = (FSplineBrushEdMode*)GLevelEditorModeTools().GetActiveMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);
	if(myMode->CurrentStack.empty())
	{
		UE_LOG(LogTemp,Warning,TEXT("can not create"));
	}
	else
	{
		//反转待加入的point
		std::stack<AMyActor*> temp;
		while(!myMode->CurrentStack.empty())
		{
			temp.push(myMode->CurrentStack.top());
			myMode->CurrentStack.pop();
		}
		//创建SplineActor
		FActorSpawnParameters SpawnParameters;
		FString SplineName = "Spline";
		FString SplineTime = FString::SanitizeFloat(myMode->GetWorld()->GetRealTimeSeconds());
		FString Name = (SplineName+ SplineTime);
		SpawnParameters.Name = FName(Name);
		AMyActor* Spline = myMode->GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(),temp.top()->GetActorLocation(),FRotator::ZeroRotator,SpawnParameters);
		USplineComponent* SplineCom = NewObject<USplineComponent>(Spline,TEXT("SplineCom"));
		SplineCom->RegisterComponent();
		
		SplineCom->AttachToComponent(Spline->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		SplineCom->ClearSplinePoints();
		FVector StartLocation = temp.top()->GetActorLocation();
		int32 Index= temp.size();
		while(!temp.empty())
		{
			SplineCom->AddSplinePoint(temp.top()->GetActorLocation(),ESplineCoordinateSpace::World);
			temp.top()->Destroy();
			temp.pop();
			
		}
		int32 FirstIndex = 0;
		SplineCom->AddSplinePoint(StartLocation,ESplineCoordinateSpace::World);
		//SplineCom->SetRotationAtSplinePoint(Index - 1,SplineCom->GetRotationAtSplinePoint(FirstIndex,ESplineCoordinateSpace::World),ESplineCoordinateSpace::World);
		SplineCom->SetTangentAtSplinePoint(Index ,SplineCom->GetTangentAtSplinePoint(0,ESplineCoordinateSpace::Local),ESplineCoordinateSpace::Local);
		
		std::stack<AMyActor*>().swap(myMode->CurrentStack);
		std::stack<BrushActorInfo>().swap(myMode->RemoveStack);
	}
	return FReply::Handled();
}


FReply SSplinePalette::OnCreateMeshClicked()
{
	return FReply::Handled();
}


FReply SSplinePalette::OnCreatSplineClicked()
{
	auto myMode = (FSplineBrushEdMode*)GLevelEditorModeTools().GetActiveMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);
	if(myMode->CurrentStack.empty())
	{
		UE_LOG(LogTemp,Warning,TEXT("can not create"));
	}
	else
	{
		//反转待加入的point
		std::stack<AMyActor*> temp;
		while(!myMode->CurrentStack.empty())
		{
			temp.push(myMode->CurrentStack.top());
			myMode->CurrentStack.pop();
		}

		UClass* BluePrintVar = StaticLoadClass(AActor::StaticClass(),nullptr,TEXT("BluePrint/BP_Actor"));
		if(BluePrintVar !=nullptr)
		{
			AMyActor * actor = myMode->GetWorld()->SpawnActor<AMyActor>(BluePrintVar);
		}
		//创建SplineActor
		 FActorSpawnParameters SpawnParameters;
		 FString SplineName = "Spline";
		 FString SplineTime = FString::SanitizeFloat(myMode->GetWorld()->GetRealTimeSeconds());
		 FString Name = (SplineName+ SplineTime);
		 SpawnParameters.Name = FName(Name);
		 AMyActor* Spline = myMode->GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(),temp.top()->GetActorLocation(),FRotator::ZeroRotator,SpawnParameters);
		 USplineComponent* SplineCom = NewObject<USplineComponent>(Spline,TEXT("SplineCom"));
		 SplineCom->RegisterComponent();
		 SplineCom->AttachToComponent(Spline->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		 SplineCom->ClearSplinePoints();
		 Spline->AddInstanceComponent(SplineCom);
		 while(!temp.empty())
		 {
		 	SplineCom->AddSplinePoint(temp.top()->GetActorLocation(),ESplineCoordinateSpace::World);
		 	temp.top()->Destroy();
		 	temp.pop();
		 }
		
		 std::stack<AMyActor*>().swap(myMode->CurrentStack);
		 std::stack<BrushActorInfo>().swap(myMode->RemoveStack);
	}
	return FReply::Handled();
}


FReply SSplinePalette::OnCloseButtonClicked()
{
	auto myMode = (FSplineBrushEdMode*)GLevelEditorModeTools().GetActiveMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);
	myMode->bToolActive =!myMode->bToolActive;
	return FReply::Handled();
}


FText SSplinePalette::GetBrushActorName() const
{
	auto myMode = (FSplineBrushEdMode*)GLevelEditorModeTools().GetActiveMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);
	return myMode->GetBrushActorName();
}

FText SSplinePalette::GetIsActive() const
{
	auto myMode = (FSplineBrushEdMode*)GLevelEditorModeTools().GetActiveMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);
	if(myMode->bToolActive)
	{
		FText out = FText::FromString("true");
		return out;
	}
	else
	{
		FText out = FText::FromString("false");
		return out;
	}
}


TSharedRef<SWidget> SSplinePalette::GetActorPicker()
{
	TArray<const UClass*> ClassFilters;
	ClassFilters.Add(UStaticMesh::StaticClass());
	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		FAssetData(),
		false,
		ClassFilters,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(ClassFilters),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateSP(this,&SSplinePalette::ChangeBrushActorType),
		FSimpleDelegate());
}

void SSplinePalette::ChangeBrushActorType(const FAssetData& AssetData)
{
	if (SelectActorCombo.IsValid())
	{
		SelectActorCombo->SetIsOpen(false);
	}
	UObject* Asset = AssetData.GetAsset();

	UStaticMesh* StaticMesh = Cast<UStaticMesh>(Asset);

	if(StaticMesh)
	{
		auto myMode = (FSplineBrushEdMode*)GLevelEditorModeTools().GetActiveMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);
		myMode->BrushActorName = FText::FromString(StaticMesh->GetName());
		myMode->BrushActor = StaticMesh;
		
	}
	//change brush actoor;

}






