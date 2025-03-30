#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>

class Material
{
public:
	Material(DirectX::XMFLOAT3 _colorTint, float _roughness = 1.0f, float _metal = 0.0f, DirectX::XMFLOAT2 _uvScale = DirectX::XMFLOAT2(1, 1), DirectX::XMFLOAT2 _uvOffset = DirectX::XMFLOAT2(0, 0));
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs();
	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot);
	void FinalizeMaterial();

	DirectX::XMFLOAT2 GetUVScale();
	DirectX::XMFLOAT2 GetUVOffset();
	DirectX::XMFLOAT3 GetColorTint();
	float GetRoughness();
	float GetMetal();

	void SetUVScale(DirectX::XMFLOAT2 scale);
	void SetUVOffset(DirectX::XMFLOAT2 offset);
	void SetColorTint(DirectX::XMFLOAT3 tint);
	void SetRoughness(float _roughness);
	void SetMetal(float _metal);

private:
	DirectX::XMFLOAT3 colorTint;
	float roughness;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
	float metal;

	bool finalized;
	int highestSRVSlot;

	//D3D12_GPU_DESCRIPTOR_HANDLE for the first of srv in the heap
	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;
	//An array of four D3D12_CPU_DESCRIPTOR_HANDLES for textures
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[4];

};