// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "libassimp/include/Importer.hpp"
#include "libassimp/include/scene.h"
#include "libassimp/include/postprocess.h"
#include "Components/ActorComponent.h"
#include "ProceduralMeshComponent.h"
#include "ModelReaderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MODELVIEWER_API UModelReaderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
		bool OpenMesh(FString Path, int32& SectionCount, FString& ErrorCode);

		bool GetSection(int32 Index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FProcMeshTangent>& Tangents);

		void Clear();

public:	
	// Sets default values for this component's properties
	UModelReaderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int32 _selectedVertex;
	int32 _meshCurrentlyProcessed;
	bool _requiresFullRecreation;

	TArray<TArray<FVector>> _vertices;
	TArray<TArray<int32>> _indices;
	TArray<TArray<FVector>> _normals;
	TArray<TArray<FVector2D>> _uvs;
	TArray<TArray<FProcMeshTangent>> _tangents;
	TArray<TArray<FColor>> _vertexColors;

	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	void ProcessNode(aiNode* node, const aiScene* scene);
};
