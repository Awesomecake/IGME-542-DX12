#pragma once

#include <d3d12.h>
#include <wrl/client.h>
#include "Camera.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Lights.h"

#include <memory>
#include <vector>

class Game
{
public:
	// Basic OOP setup
	Game() = default;
	~Game();
	Game(const Game&) = delete; // Remove copy constructor
	Game& operator=(const Game&) = delete; // Remove copy-assignment operator

	// Primary functions
	void Initialize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void OnResize();

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void CreateGeometry();

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Geometry
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW ibView{};

	// Other graphics data
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

	std::shared_ptr<Camera> camera;

	std::vector<std::shared_ptr<GameEntity>> entities;

	std::shared_ptr<Mesh> cube;
	std::shared_ptr<Mesh> cylinder;
	std::shared_ptr<Mesh> helix;
	std::shared_ptr<Mesh> sphere;
	std::shared_ptr<Mesh> torus;
	std::shared_ptr<Mesh> quad;

	std::shared_ptr<Material> greyDiffuse;
	std::shared_ptr<Material> darkGrey;
	std::shared_ptr<Material> metal;

	std::shared_ptr<Material> cobblestone;
	std::shared_ptr<Material> scratched;
	std::shared_ptr<Material> bronze;
	std::shared_ptr<Material> floor;
	std::shared_ptr<Material> paint;
	std::shared_ptr<Material> iron;
	std::shared_ptr<Material> wood;

	std::vector<Light> lights;
};

