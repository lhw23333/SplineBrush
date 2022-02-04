// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

#include <Actor.h>
#include <algorithm>


#include "Components/SplineComponent.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("staticMesh");
	RootComponent = StaticMesh;
	
}
float AMyActor::MeshDistance = 10;
UStaticMesh* AMyActor::NeedCreateMesh = nullptr;
// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FReply AMyActor::OnCreateMeshClicked()
{
	if(GetComponentByClass(USplineComponent::StaticClass()))
	{
		UE_LOG(LogTemp,Warning,TEXT("查找到SplineComponent"));
		USplineComponent* Spline = Cast<USplineComponent>(GetComponentByClass(USplineComponent::StaticClass()));
		float SplineLength =  Spline->GetSplineLength();
		
		int Count = SplineLength/MeshDistance;
		UE_LOG(LogTemp,Warning,TEXT("%d"),Count);

		for(int i = 0 ;i<= Count ;i++)
		{
			
			UE_LOG(LogTemp,Warning,TEXT("%f"),i  * MeshDistance);
			FString MeshName = "MesCom"+ FString::SanitizeFloat(i);
			
			FVector StartLocation = Spline->GetLocationAtDistanceAlongSpline(MeshDistance* i,ESplineCoordinateSpace::Local);
			FVector LookLocation = Spline->GetLocationAtDistanceAlongSpline(MeshDistance* i *1.1f,ESplineCoordinateSpace::Local);
			FRotator ComRotation = (LookLocation - StartLocation).Rotation();
			UStaticMeshComponent * StaticMeshCom = NewObject<UStaticMeshComponent>(this,FName(MeshName));
			StaticMeshCom->RegisterComponent();
			StaticMeshCom->SetStaticMesh(NeedCreateMesh);
			StaticMeshCom->SetWorldLocation(Spline->GetLocationAtDistanceAlongSpline(MeshDistance * i, ESplineCoordinateSpace::Local));
			StaticMeshCom->SetWorldRotation(ComRotation);
			StaticMeshCom->AttachToComponent(this->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);

			this->AddInstanceComponent(StaticMeshCom);
		}
		
		
		
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("未查找到SplineComponent"));
	}
	
	return FReply::Handled();
}






