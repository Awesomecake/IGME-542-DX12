#include "Material.h"
#include "Graphics.h"

Material::Material()
{

}

Material::Material(Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState, DirectX::XMFLOAT3 _colorTint, DirectX::XMFLOAT2 _uvScale, DirectX::XMFLOAT2 _uvOffset) 
{ 
	pipelineState = _pipelineState;
	colorTint = _colorTint;
	uvScale = _uvScale;
	uvOffset = _uvOffset;
	finalized = false;
}

D3D12_GPU_DESCRIPTOR_HANDLE Material::GetFinalGPUHandleForSRVs()
{
	return finalGPUHandleForSRVs;
}

Microsoft::WRL::ComPtr<ID3D12PipelineState> Material::GetPipelineState()
{
	return pipelineState;
}

DirectX::XMFLOAT2 Material::GetUVScale()
{
	return uvScale;
}

DirectX::XMFLOAT2 Material::GetUVOffset()
{
	return uvOffset;
}

//Register SRV into Slot - Currently only 4 slots available
void Material::AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot)
{
	textureSRVsBySlot[slot] = srv;
}

void Material::FinalizeMaterial()
{
	for (size_t i = 0; i < 4; i++)
	{
		if (i == 0)
			finalGPUHandleForSRVs = Graphics::CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[i], 1);
		else
			Graphics::CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[i], 1);
	}

	finalized = true;
}