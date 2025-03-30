#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

#include <memory>

class GameEntity
{
private:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

public:

	GameEntity(std::shared_ptr<Mesh> refMesh, std::shared_ptr<Material> mat);
	~GameEntity();
	std::shared_ptr<Mesh> GetMesh();
	std::shared_ptr<Transform> GetTransform();
	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> mat);
};