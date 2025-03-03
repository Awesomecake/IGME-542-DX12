#include "Material.h"
#include "Graphics.h"

Material::Material( DirectX::XMFLOAT3 _colorTint, DirectX::XMFLOAT2 _uvScale, DirectX::XMFLOAT2 _uvOffset) 
{ 
	colorTint = _colorTint;
	uvScale = _uvScale;
	uvOffset = _uvOffset;
	finalized = false;
}

D3D12_GPU_DESCRIPTOR_HANDLE Material::GetFinalGPUHandleForSRVs()
{
	return finalGPUHandleForSRVs;
}

DirectX::XMFLOAT2 Material::GetUVScale()
{
	return uvScale;
}

DirectX::XMFLOAT2 Material::GetUVOffset()
{
	return uvOffset;
}

DirectX::XMFLOAT3 Material::GetColorTint()
{
	return colorTint;
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