#include "Material.h"
#include "Graphics.h"

Material::Material( DirectX::XMFLOAT3 _colorTint, float _roughness, float _metal, DirectX::XMFLOAT2 _uvScale, DirectX::XMFLOAT2 _uvOffset) 
{ 
	colorTint = _colorTint;
	roughness = _roughness;
	metal = _metal;
	uvScale = _uvScale;
	uvOffset = _uvOffset;
	finalized = false;

	// Init remaining data
	finalGPUHandleForSRVs = {};
	ZeroMemory(textureSRVsBySlot, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE) * 4);
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

float Material::GetRoughness()
{
	return roughness;
}

float Material::GetMetal()
{
	return metal;
}

void Material::SetUVScale(DirectX::XMFLOAT2 scale)
{
	uvScale = scale;
}

void Material::SetUVOffset(DirectX::XMFLOAT2 offset)
{
	uvOffset = offset;
}

void Material::SetColorTint(DirectX::XMFLOAT3 tint)
{
	colorTint = tint;
}

void Material::SetRoughness(float _roughness)
{
	roughness = _roughness;
}

void Material::SetMetal(float _metal)
{
	metal = _metal;
}


//Register SRV into Slot - Currently only 4 slots available
void Material::AddTexture(D3D12_CPU_DESCRIPTOR_HANDLE srv, int slot)
{
	// Valid slot?
	if (finalized || slot < 0 || slot >= 4)
		return;

	// Save and check if this was the highest slot
	textureSRVsBySlot[slot] = srv;
	highestSRVSlot = max(highestSRVSlot, slot);
}

void Material::FinalizeMaterial()
{
	if (!finalized)
	{
		for (int i = 0; i <= highestSRVSlot; i++)
		{
			// Copy a single SRV at a time since they're all
			// currently in separate heaps!
			D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle =
				Graphics::CopySRVsToDescriptorHeapAndGetGPUDescriptorHandle(textureSRVsBySlot[i], 1);

			// Save the first resulting handle
			if (i == 0) { finalGPUHandleForSRVs = gpuHandle; }
		}

		finalized = true;
	}
}