

#include "DetailCustomizations.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#pragma once

class FMyClassDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails( IDetailLayoutBuilder& DetailBuilder ) override;

	TSharedRef<SWidget> GetActorPicker();

	TSharedPtr<class SComboButton> SelectActorCombo;
	
	void ChangeNeedCreateMeshType(const FAssetData& AssetData);
	
	TAttribute<float> MeshDiatance;

	TAttribute<FText> NeedCreateMeshName;

	FText GetNeedCreateMeshName() const;
	
	float GetMeshDistance()const;
};
