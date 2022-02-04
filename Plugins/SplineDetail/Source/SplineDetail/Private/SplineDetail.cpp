// Copyright Epic Games, Inc. All Rights Reserved.

#include "SplineDetail.h"

#include "MyClassDetails.h"


#define LOCTEXT_NAMESPACE "FSplineDetailModule"

void FSplineDetailModule::StartupModule()
{

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(FName("MyObject"), FOnGetDetailCustomizationInstance::CreateStatic(&FMyClassDetails::MakeInstance));

	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSplineDetailModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomClassLayout(FName("MyObject"));
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSplineDetailModule, SplineDetail)