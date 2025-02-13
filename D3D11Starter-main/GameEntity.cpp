#include "GameEntity.h"
#include "BufferStructs.h"
#include "Graphics.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> refMesh, std::shared_ptr<Material> mat)
{
	material = mat;
	mesh = refMesh;
}

GameEntity::~GameEntity()
{

}

Transform& GameEntity::GetTransform()
{
	return transform;
}

std::shared_ptr<Mesh> GameEntity::GetMesh()
{
	return mesh;
}

std::shared_ptr<Material> GameEntity::GetMaterial()
{
	return material;
}

void GameEntity::SetMaterial(std::shared_ptr<Material> mat)
{
	material = mat;
}

void GameEntity::Draw(std::shared_ptr<Camera> camera)
{
	VertexShaderExternalData newData;
	newData.projection = camera->GetProjectionMatrix();
	newData.world = transform.GetWorldMatrix();
	newData.view = camera->GetViewMatrix();
	newData.worldInvTranspose = transform.GetWorldInverseTransposeMatrix();

	D3D12_GPU_DESCRIPTOR_HANDLE handle = Graphics::FillNextConstantBufferAndGetGPUDescriptorHandle(&newData, sizeof(VertexShaderExternalData));
	Graphics::CommandList->SetGraphicsRootDescriptorTable(0, handle);
	
	mesh->Draw();
}