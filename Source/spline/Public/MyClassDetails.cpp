#include "MyClassDetails.h"

#include "IDetailCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#define LOCTEXT_NAMESPACE "MyClassDetails"

TSharedRef<IDetailCustomization> FMyClassDetails::MakeInstance()
{
	return MakeShareable(new FMyClassDetails);
}

void FMyClassDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& CustCategory  = DetailBuilder.EditCategory("Aaa");
	CustCategory.AddCustomRow(LOCTEXT("CustRow", "CustRow"), false).NameContent()   //NameContent是属性名的显示内容
		[
			SNew(STextBlock).Text(LOCTEXT("TextBlock", "TextBlock"))
		].ValueContent()
		[   //ValueContent是值的显示内容。如果不想区分Key和Value分割的话，可以直接使用WholeRowContent，下面有效果。
			SNew(SVerticalBox)+SVerticalBox::Slot().HAlign(HAlign_Fill).AutoHeight()
			[
				SNew(SCheckBox)
			]
		];
	
}

#undef LOCTEXT_NAMESPACE
