#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>

class Material
{
public:
	Material(DirectX::XMFLOAT3 _colorTint, DirectX::XMFLOAT2 _uvScale, DirectX::XMFLOAT2 _uvOffset);
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs();
	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot);
	void FinalizeMaterial();

	DirectX::XMFLOAT2 GetUVScale();
	DirectX::XMFLOAT2 GetUVOffset();
	DirectX::XMFLOAT3 GetColorTint();

private:
	DirectX::XMFLOAT3 colorTint;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
	bool finalized;

	//D3D12_GPU_DESCRIPTOR_HANDLE for the first of srv in the heap
	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;
	//An array of four D3D12_CPU_DESCRIPTOR_HANDLES for textures
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[4];

};