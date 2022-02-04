// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

UCLASS()
class  AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	
public:	

	static  float MeshDistance;
	UPROPERTY()
	USceneComponent* Root;

	UPROPERTY()
	UStaticMeshComponent* StaticMesh;



	FReply OnCreateMeshClicked();


	static  UStaticMesh* NeedCreateMesh;

	static float MinPitch;

	static float MaxPitch;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
