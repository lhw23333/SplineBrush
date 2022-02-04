// Copyright Epic Games, Inc. All Rights Reserved.

#include "spline.h"

#include "MyClassDetails.h"
#include "TestActor.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FTestActorDetailModule, spline, "spline" );


void FTestActorDetailModule::StartupModule()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(ATestActor::StaticClass()->GetFName(), FOnGetDetailCustomizationInstance::CreateStatic(&FMyClassDetails::MakeInstance));

}

void FTestActorDetailModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomClassLayout(ATestActor::StaticClass()->GetFName());
	//
}
