#pragma once
#include <DirectXMath.h>
#include "Lights.h"

#define MAX_NUM_LIGHTS 10

struct VertexShaderExternalData
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
	DirectX::XMFLOAT4X4 worldInvTranspose;
};

struct PixelShaderExternalData
{
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
	DirectX::XMFLOAT3 cameraPos;
	int lightCount;
	Light lights[MAX_NUM_LIGHTS];
};

// Overall scene data for raytracing
struct RaytracingSceneData
{
	DirectX::XMFLOAT4X4 inverseViewProjection;
	DirectX::XMFLOAT3 cameraPosition;
	float pad;
};

// All material data for raytracing
struct RaytracingMaterialData
{
	// 16 bytes
	DirectX::XMFLOAT3 color;
	float roughness;

	// 16 bytes
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;

	// 16 bytes
	float metal;
	DirectX::XMFLOAT3 padding;

	// 16 bytes
	unsigned int albedoIndex;
	unsigned int normalMapIndex;
	unsigned int roughnessIndex;
	unsigned int metalnessIndex;
};


// Ensure this matches Raytracing shader define!
#define MAX_INSTANCES_PER_BLAS 100
struct RaytracingEntityData
{
	RaytracingMaterialData materials[MAX_INSTANCES_PER_BLAS];
};