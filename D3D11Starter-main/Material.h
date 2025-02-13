#pragma once
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl/client.h>

class Material
{
public:
	Material();
	Material(Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState, DirectX::XMFLOAT3 _colorTint, DirectX::XMFLOAT2 _uvScale, DirectX::XMFLOAT2 _uvOffset);
	D3D12_GPU_DESCRIPTOR_HANDLE GetFinalGPUHandleForSRVs();
	void AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot);
	void FinalizeMaterial();

	Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState();
	DirectX::XMFLOAT2 GetUVScale();
	DirectX::XMFLOAT2 GetUVOffset();

private:
	DirectX::XMFLOAT3 colorTint;
	DirectX::XMFLOAT2 uvScale;
	DirectX::XMFLOAT2 uvOffset;
	bool finalized;

	//An ID3D12PipelineState, which replaces individual vertex and pixel shaders
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	//D3D12_GPU_DESCRIPTOR_HANDLE for the first of srv in the heap
	D3D12_GPU_DESCRIPTOR_HANDLE finalGPUHandleForSRVs;
	//An array of four D3D12_CPU_DESCRIPTOR_HANDLES for textures
	D3D12_CPU_DESCRIPTOR_HANDLE textureSRVsBySlot[4];

};