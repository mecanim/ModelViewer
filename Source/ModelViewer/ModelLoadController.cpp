// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelLoadController.h"

// Sets default values
AModelLoadController::AModelLoadController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	meshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	readerComponent = CreateDefaultSubobject<UModelReaderComponent>(TEXT("ModelFileReader"));
}

// Called when the game starts or when spawned
void AModelLoadController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AModelLoadController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isRotating)
	{
		//meshComponent->AddWorldRotation(FRotator(0.f, _rotationSpeedScalar, 0.f));
		meshComponent->AddLocalRotation(FRotator(0.f, _rotationSpeedScalar, 0.f));
	}
}

// Load external mesh on scene
bool AModelLoadController::LoadMesh(FString Path)
{
	meshComponent->ClearAllMeshSections();
	readerComponent->Clear();

	int32 sectionCount;
	FString error;

	if (!readerComponent->OpenMesh(Path, sectionCount, error))
	{
		OnLoadFailed(error);
		return false;
	}

	TArray<FVector> Vertices;
	TArray<int32> Faces;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FLinearColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	for (int32 i = 0; i < sectionCount; ++i)
	{	
		readerComponent->GetSection(i, Vertices, Faces, Normals, UV, Tangents);
		meshComponent->ContainsPhysicsTriMeshData(true);
		meshComponent->CreateMeshSection_LinearColor(i, Vertices, Faces, Normals, UV, Colors, Tangents, true);
	}

	return true;
}

// Method to get signal from widget to start/stop rotation
void AModelLoadController::StartRotation(float Value)
{
	isRotating = (Value == 0.f) ? false : true;
	if (isRotating)
	{
		_rotationSpeedScalar *= Value;
	}
	else
	{
		_rotationSpeedScalar = _rotScalar;
	}
}