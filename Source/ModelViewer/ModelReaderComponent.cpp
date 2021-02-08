// Fill out your copyright notice in the Description page of Project Settings.


#include "ModelReaderComponent.h"
using namespace Assimp;

// Sets default values for this component's properties
UModelReaderComponent::UModelReaderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UModelReaderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UModelReaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Try to read mesh from file
bool UModelReaderComponent::OpenMesh(FString Path, int32& SectionCount, FString& ErrorCode)
{
	Importer importer; 
	std::string filename(TCHAR_TO_UTF8(*Path));
	const aiScene* scene = importer.ReadFile(filename, aiProcessPreset_TargetRealtime_MaxQuality);
	if (!scene)
	{
		ErrorCode = importer.GetErrorString();
		return false;
	}
	_meshCurrentlyProcessed = 0;
	ProcessNode(scene->mRootNode, scene);
	SectionCount = _meshCurrentlyProcessed;

	return true;
}

// Get vertices by section
bool UModelReaderComponent::GetSection(int32 Index, TArray<FVector>& Vertices, TArray<int32>& Faces, TArray<FVector>& Normals, TArray<FVector2D>& UV, TArray<FProcMeshTangent>& Tangents)
{
	if (Index >= _meshCurrentlyProcessed)
	{
		return false;
	}

	Vertices = _vertices[Index];
	Faces = _indices[Index];
	Normals = _normals[Index];
	UV = _uvs[Index];
	Tangents = _tangents[Index];

	return true;
}

void UModelReaderComponent::Clear()
{
	_vertices.Empty();
	_indices.Empty();
	_normals.Empty();
	_uvs.Empty();
	_tangents.Empty();
	_vertexColors.Empty();
	_meshCurrentlyProcessed = 0;
}

// Dive into nested structure and get geom. data
void UModelReaderComponent::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (uint32 i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene);
		++_meshCurrentlyProcessed;
	}
	
	for (uint32 i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

// Get data from final nested elemetn - mesh
void UModelReaderComponent::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	if (_vertices.Num() <= _meshCurrentlyProcessed)
	{
		_vertices.AddZeroed();
		_normals.AddZeroed();
		_uvs.AddZeroed();
		_tangents.AddZeroed();
		_vertexColors.AddZeroed();
		_indices.AddZeroed();
	}

	if (mesh->mNumVertices != _vertices[_meshCurrentlyProcessed].Num())
	{
		_requiresFullRecreation = true;
	}

	_vertices[_meshCurrentlyProcessed].Empty();
	_normals[_meshCurrentlyProcessed].Empty();
	_uvs[_meshCurrentlyProcessed].Empty();
	_tangents[_meshCurrentlyProcessed].Empty();
	_vertexColors[_meshCurrentlyProcessed].Empty();
	_indices[_meshCurrentlyProcessed].Empty();

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		FVector vertex, normal;
		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;
		
		if (mesh->mTextureCoords[0])
		{
			FVector2D uvs;
			uvs.X = mesh->mTextureCoords[0][i].x;
			uvs.Y = mesh->mTextureCoords[0][i].y;
			_uvs[_meshCurrentlyProcessed].Add(uvs);
		}
		else
		{
			_uvs[_meshCurrentlyProcessed].Add(FVector2D());
		}
		_vertices[_meshCurrentlyProcessed].Add(vertex);

		normal.X = mesh->mNormals[i].x;
		normal.Y = mesh->mNormals[i].y;
		normal.Z = mesh->mNormals[i].z;
		_normals[_meshCurrentlyProcessed].Add(normal);
	}

	if (_requiresFullRecreation)
	{
		for (uint32 i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[2]);
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[1]);
			_indices[_meshCurrentlyProcessed].Add(face.mIndices[0]);
		}
	}
}