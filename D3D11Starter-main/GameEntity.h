#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include "Material.h"

#include <memory>

class GameEntity
{
private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;

public:

	GameEntity(std::shared_ptr<Mesh> refMesh, std::shared_ptr<Material> mat);
	~GameEntity();
	std::shared_ptr<Mesh> GetMesh();
	Transform& GetTransform();
	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> mat);
	void Draw(std::shared_ptr<Camera> camera);

};