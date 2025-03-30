#include "GameEntity.h"
#include "BufferStructs.h"
#include "Graphics.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> refMesh, std::shared_ptr<Material> mat)
{
	material = mat;
	mesh = refMesh;
	transform = std::make_shared<Transform>();
}

GameEntity::~GameEntity()
{

}

std::shared_ptr<Transform> GameEntity::GetTransform()
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