// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ModelReaderComponent.h"
#include "ModelLoadController.generated.h"

UCLASS(Blueprintable)
class MODELVIEWER_API AModelLoadController : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	UModelReaderComponent* readerComponent;
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* meshComponent;

	UFUNCTION(BlueprintCallable, Category = "Loading")
	bool LoadMesh(FString Path);

	//Event to inform user through widget about mesh file load failed
	UFUNCTION(BlueprintImplementableEvent, Category = "Loading")
	void OnLoadFailed(const FString& OutError);

	UFUNCTION(BlueprintCallable, Category = "Loading")
	void StartRotation(float Value);

public:	
	// Sets default values for this actor's properties
	AModelLoadController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool isRotating;
	const float _rotScalar = 2.f;
	float _rotationSpeedScalar = _rotScalar;
};
