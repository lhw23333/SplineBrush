#include "MyClassDetails.h"

#include "IDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "MyActor.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SSpinBox.h"
#define LOCTEXT_NAMESPACE "MyClassDetails"

TSharedRef<IDetailCustomization> FMyClassDetails::MakeInstance()
{
	return MakeShareable(new FMyClassDetails);
}

void FMyClassDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
	DetailBuilder.GetObjectsBeingCustomized(CustomizedObjects);
	
	IDetailCategoryBuilder& CustCategory  = DetailBuilder.EditCategory("Auto Mesh");
	CustCategory.AddCustomRow(LOCTEXT("CustRow", "CustRow"), false).NameContent()   //NameContent是属性名的显示内容
		[
			SNew(STextBlock).Text(FText::GetEmpty())
		].ValueContent()
		[   //ValueContent是值的显示内容。如果不想区分Key和Value分割的话，可以直接使用WholeRowContent，下面有效果。
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.Text(LOCTEXT("ButtonText","Create Mesh"))
				.OnClicked_Lambda([CustomizedObjects]()
				{
					for(auto Object : CustomizedObjects)
					{
						if(Object.IsValid() && Object->IsA(AMyActor::StaticClass()))
						{
							Cast<AMyActor>(Object)->OnCreateMeshClicked();
						}
					}
					return FReply::Handled();
				})
			]
		];
	
	MeshDiatance.Bind(this,&FMyClassDetails::GetMeshDistance);
	CustCategory.AddCustomRow(LOCTEXT("CustRow", "CustRow"), false).NameContent()
	[
		SNew(STextBlock).Text(FText::FromString("MeshDistance"))
	].ValueContent()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SSpinBox<float>)
			.Value(MeshDiatance)
			.OnValueChanged_Lambda([](float InValue)
			{
				AMyActor::MeshDistance = InValue;
			})
		]
	];
	NeedCreateMeshName.Bind(this,&FMyClassDetails::GetNeedCreateMeshName);
	CustCategory.AddCustomRow(LOCTEXT("CustRow", "CustRow"), false).NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Chosen Mesh"))
	].ValueContent()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SAssignNew(SelectActorCombo,SComboButton)
			.ForegroundColor(FLinearColor::White)
			.ButtonStyle(FEditorStyle::Get(), "FlatButton.Success")
			.OnGetMenuContent(this, &FMyClassDetails::GetActorPicker)
			.ContentPadding(FMargin(1.f))
			.ButtonContent()
			[
				SNew(STextBlock)
				.Text(NeedCreateMeshName)
			]
		]
	];

	LimitMinPitch.Bind(this,&FMyClassDetails::GetMinPich);
	CustCategory.AddCustomRow(LOCTEXT("CustRow", "CustRow"), false).NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Limit MinPitch"))
	].ValueContent()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SSpinBox<float>)
			.Value(LimitMinPitch)
			.OnValueChanged_Lambda([](float InValue)
			{
				AMyActor::MinPitch = InValue;
			})
		]
	];

	LimitMaxPitch.Bind(this,&FMyClassDetails::GetMaxPitch);
	CustCategory.AddCustomRow(LOCTEXT("CustRow", "CustRow"), false).NameContent()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Limit MinPitch"))
	].ValueContent()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(SSpinBox<float>)
			.Value(LimitMaxPitch)
			.OnValueChanged_Lambda([](float InValue)
			{
				AMyActor::MaxPitch = InValue;
			})
		]
	];
	
	
}

TSharedRef<SWidget> FMyClassDetails::GetActorPicker()
{
	TArray<const UClass*> ClassFilters;
	ClassFilters.Add(UStaticMesh::StaticClass());
	return PropertyCustomizationHelpers::MakeAssetPickerWithMenu(
		FAssetData(),
		false,
		ClassFilters,
		PropertyCustomizationHelpers::GetNewAssetFactoriesForClasses(ClassFilters),
		FOnShouldFilterAsset(),
		FOnAssetSelected::CreateSP(this,&FMyClassDetails::ChangeNeedCreateMeshType),
		FSimpleDelegate());
}

void FMyClassDetails::ChangeNeedCreateMeshType(const FAssetData& AssetData)
{
	if (SelectActorCombo.IsValid())
	{
		SelectActorCombo->SetIsOpen(false);
	}
	UObject* Asset = AssetData.GetAsset();

	UStaticMesh* StaticMesh = Cast<UStaticMesh>(Asset);

	if(StaticMesh)
	{
		AMyActor::NeedCreateMesh = StaticMesh;
	}
}


float FMyClassDetails::GetMaxPitch()const
{
	return AMyActor::MaxPitch;
}


float FMyClassDetails::GetMinPich()const
{
	return AMyActor::MinPitch;
}


float FMyClassDetails::GetMeshDistance() const
{
	return AMyActor::MeshDistance;
}

FText FMyClassDetails::GetNeedCreateMeshName() const
{
	if(AMyActor::NeedCreateMesh == nullptr)
	{
		return FText::GetEmpty();
	}
	else
	{
		return FText::FromString(AMyActor::NeedCreateMesh->GetName());
	}
	
}


#undef LOCTEXT_NAMESPACE
