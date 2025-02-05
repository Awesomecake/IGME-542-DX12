#include "GameEntity.h"
#include "BufferStructs.h"
#include "Graphics.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> refMesh)
{
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

void GameEntity::Draw(std::shared_ptr<Camera> camera)
{
	VertexShaderExternalData newData;
	newData.projection = camera->GetProjectionMatrix();
	newData.world = transform.GetWorldMatrix();
	newData.view = camera->GetViewMatrix();

	D3D12_GPU_DESCRIPTOR_HANDLE handle = Graphics::FillNextConstantBufferAndGetGPUDescriptorHandle(&newData, sizeof(VertexShaderExternalData));
	Graphics::CommandList->SetGraphicsRootDescriptorTable(0, handle);
	
	mesh->Draw();
}