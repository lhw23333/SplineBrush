#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Framework/Text/SlateHyperlinkRun.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Views/SHeaderRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STileView.h"
#include "Widgets/Views/STreeView.h"
#include "Misc/TextFilter.h"
#include "SplineBrushEdMode.h"
#include "Components/SplineComponent.h"

class SSplinePalette: public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SSplinePalette) {}
		SLATE_ARGUMENT(FSplineBrushEdMode*, SplineBrushEdMode)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	~SSplinePalette();

	TSharedRef<SWidget> Update(FText test);
	TSharedPtr<class SComboButton> SelectActorCombo;
	TSharedPtr<class STextBlock> Stext;
		
	TAttribute<FText> BrushActorName;

	FText GetBrushActorName() const;

	TAttribute<FText> IsActive;



	
	FText GetIsActive()const;

	FReply OnCloseButtonClicked();
	
	TSharedRef<SWidget> GetActorPicker();

	void ChangeBrushActorType(const FAssetData& AssetData);

	FReply OnCreatSplineClicked();

	FReply OnCreatEndToEndSplineClicked();

	FReply OnCreateMeshClicked();
	
	
;
};
