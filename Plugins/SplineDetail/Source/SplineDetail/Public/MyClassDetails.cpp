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
	IDetailCategoryBuilder& LightingCategory  = DetailBuilder.EditCategory("Lighting", FText::GetEmpty());
	LightingCategory.AddProperty(FName("bCastStaticShadow"));
	
}

#undef LOCTEXT_NAMESPACE
