// Copyright Epic Games, Inc. All Rights Reserved.

#include "SplineBrush.h"


#include "MyClassDetails.h"
#include "SplineBrushEdMode.h"

#define LOCTEXT_NAMESPACE "FSplineBrushModule"

void FSplineBrushModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FSplineBrushEdMode>(FSplineBrushEdMode::EM_SplineBrushEdModeId, LOCTEXT("SplineBrushEdModeName", "SplineBrushEdMode"), FSlateIcon(), true);
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(AMyActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FMyClassDetails::MakeInstance));

	
}

void FSplineBrushModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FSplineBrushEdMode::EM_SplineBrushEdModeId);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomClassLayout(AMyActor::StaticClass()->GetFName());

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSplineBrushModule, SplineBrush)