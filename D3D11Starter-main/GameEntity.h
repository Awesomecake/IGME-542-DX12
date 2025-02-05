#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"

#include <memory>

class GameEntity
{
private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;

public:

	GameEntity(std::shared_ptr<Mesh> refMesh);
	~GameEntity();
	std::shared_ptr<Mesh> GetMesh();
	Transform& GetTransform();
	void Draw(std::shared_ptr<Camera> camera);

};