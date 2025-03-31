#include "Game.h"
#include "Graphics.h"
#include "Vertex.h"
#include "Input.h"
#include "PathHelpers.h"
#include "Window.h"
#include "BufferStructs.h"
#include "RayTracing.h"

#include <DirectXMath.h>

// Needed for a helper function to load pre-compiled shader files
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

#define PBR_Assets L"../../Assets/PBR/"

// --------------------------------------------------------
// Called once per program, after the window and graphics API
// are initialized but before the game loop begins
// --------------------------------------------------------
void Game::Initialize()
{
	// Initialize raytracing
	RayTracing::Initialize(Window::Width(), Window::Height(), FixPath(L"RayTracing.cso"));

	camera = std::make_shared<Camera>(viewport.Width / viewport.Height, 45.f, XMFLOAT3(0.5, 0, -5));
	entities = std::vector<std::shared_ptr<GameEntity>>();

	// Load textures
	D3D12_CPU_DESCRIPTOR_HANDLE cobblestoneAlbedo = Graphics::LoadTexture(FixPath(PBR_Assets "cobblestone_albedo.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE cobblestoneNormals = Graphics::LoadTexture(FixPath(PBR_Assets "cobblestone_normals.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE cobblestoneRoughness = Graphics::LoadTexture(FixPath(PBR_Assets "cobblestone_roughness.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE cobblestoneMetal = Graphics::LoadTexture(FixPath(PBR_Assets "cobblestone_metal.png").c_str());

	D3D12_CPU_DESCRIPTOR_HANDLE bronzeAlbedo = Graphics::LoadTexture(FixPath(PBR_Assets "bronze_albedo.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE bronzeNormals = Graphics::LoadTexture(FixPath(PBR_Assets "bronze_normals.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE bronzeRoughness = Graphics::LoadTexture(FixPath(PBR_Assets "bronze_roughness.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE bronzeMetal = Graphics::LoadTexture(FixPath(PBR_Assets "bronze_metal.png").c_str());

	D3D12_CPU_DESCRIPTOR_HANDLE scratchedAlbedo = Graphics::LoadTexture(FixPath(PBR_Assets "scratched_albedo.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE scratchedNormals = Graphics::LoadTexture(FixPath(PBR_Assets "scratched_normals.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE scratchedRoughness = Graphics::LoadTexture(FixPath(PBR_Assets "scratched_roughness.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE scratchedMetal = Graphics::LoadTexture(FixPath(PBR_Assets "scratched_metal.png").c_str());

	D3D12_CPU_DESCRIPTOR_HANDLE woodAlbedo = Graphics::LoadTexture(FixPath(PBR_Assets "wood_albedo.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE woodNormals = Graphics::LoadTexture(FixPath(PBR_Assets "wood_normals.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE woodRoughness = Graphics::LoadTexture(FixPath(PBR_Assets "wood_roughness.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE woodMetal = Graphics::LoadTexture(FixPath(PBR_Assets "wood_metal.png").c_str());

	D3D12_CPU_DESCRIPTOR_HANDLE floorAlbedo = Graphics::LoadTexture(FixPath(PBR_Assets "floor_albedo.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE floorNormals = Graphics::LoadTexture(FixPath(PBR_Assets "floor_normals.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE floorRoughness = Graphics::LoadTexture(FixPath(PBR_Assets "floor_roughness.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE floorMetal = Graphics::LoadTexture(FixPath(PBR_Assets "floor_metal.png").c_str());

	D3D12_CPU_DESCRIPTOR_HANDLE paintAlbedo = Graphics::LoadTexture(FixPath(PBR_Assets "paint_albedo.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE paintNormals = Graphics::LoadTexture(FixPath(PBR_Assets "paint_normals.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE paintRoughness = Graphics::LoadTexture(FixPath(PBR_Assets "paint_roughness.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE paintMetal = Graphics::LoadTexture(FixPath(PBR_Assets "paint_metal.png").c_str());

	D3D12_CPU_DESCRIPTOR_HANDLE ironAlbedo = Graphics::LoadTexture(FixPath(PBR_Assets "rough_albedo.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE ironNormals = Graphics::LoadTexture(FixPath(PBR_Assets "rough_normals.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE ironRoughness = Graphics::LoadTexture(FixPath(PBR_Assets "rough_roughness.png").c_str());
	D3D12_CPU_DESCRIPTOR_HANDLE ironMetal = Graphics::LoadTexture(FixPath(PBR_Assets "rough_metal.png").c_str());

	// Create materials
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState{};
	// Create materials
	// Note: Samplers are handled by a single static sampler in the
	// root signature for this demo, rather than per-material
	greyDiffuse = std::make_shared<Material>(XMFLOAT3(0.5f, 0.5f, 0.5f), 1.0f, 0.0f);
	reflective = std::make_shared<Material>(XMFLOAT3(0.25f, 0.25f, 0.25f), 0.0f, 0.0f);
	metal = std::make_shared<Material>(XMFLOAT3(0.5f, 0.6f, 0.7f), 0.0f, 1.0f);

	cobblestone = std::make_shared<Material>(XMFLOAT3(1, 1, 1));
	scratched = std::make_shared<Material>(XMFLOAT3(1, 1, 1));
	bronze = std::make_shared<Material>(XMFLOAT3(1, 1, 1));
	floor = std::make_shared<Material>(XMFLOAT3(1, 1, 1));
	paint = std::make_shared<Material>(XMFLOAT3(1, 1, 1));
	iron = std::make_shared<Material>(XMFLOAT3(1, 1, 1));
	wood = std::make_shared<Material>(XMFLOAT3(1, 1, 1));

	// Set up textures
	cobblestone->AddTexture(cobblestoneAlbedo, 0);
	cobblestone->AddTexture(cobblestoneNormals, 1);
	cobblestone->AddTexture(cobblestoneRoughness, 2);
	cobblestone->AddTexture(cobblestoneMetal, 3);
	cobblestone->FinalizeMaterial();

	scratched->AddTexture(scratchedAlbedo, 0);
	scratched->AddTexture(scratchedNormals, 1);
	scratched->AddTexture(scratchedRoughness, 2);
	scratched->AddTexture(scratchedMetal, 3);
	scratched->FinalizeMaterial();

	bronze->AddTexture(bronzeAlbedo, 0);
	bronze->AddTexture(bronzeNormals, 1);
	bronze->AddTexture(bronzeRoughness, 2);
	bronze->AddTexture(bronzeMetal, 3);
	bronze->FinalizeMaterial();

	floor->AddTexture(floorAlbedo, 0);
	floor->AddTexture(floorNormals, 1);
	floor->AddTexture(floorRoughness, 2);
	floor->AddTexture(floorMetal, 3);
	floor->FinalizeMaterial();

	paint->AddTexture(paintAlbedo, 0);
	paint->AddTexture(paintNormals, 1);
	paint->AddTexture(paintRoughness, 2);
	paint->AddTexture(paintMetal, 3);
	paint->FinalizeMaterial();

	wood->AddTexture(woodAlbedo, 0);
	wood->AddTexture(woodNormals, 1);
	wood->AddTexture(woodRoughness, 2);
	wood->AddTexture(woodMetal, 3);
	wood->FinalizeMaterial();

	iron->AddTexture(ironAlbedo, 0);
	iron->AddTexture(ironNormals, 1);
	iron->AddTexture(ironRoughness, 2);
	iron->AddTexture(ironMetal, 3);
	iron->FinalizeMaterial();

	CreateGeometry();

	// Finalize any initialization and wait for the GPU
	// before proceeding to the game loop
	Graphics::CloseAndExecuteCommandList();
	Graphics::WaitForGPU();
	Graphics::ResetAllocatorAndCommandList();

	RayTracing::CreateTopLevelAccelerationStructureForScene(entities);

#pragma region Constructing Lights
	lights = std::vector<Light>();

	Light light3 = Light();
	light3.Direction = XMFLOAT3(0, -1, -1);
	light3.Color = XMFLOAT3(1, 1, 1);
	lights.push_back(light3);

	Light light = Light();
	light.Type = LIGHT_TYPE_POINT;
	light.Color = XMFLOAT3(1, 0, 0);
	light.Position = XMFLOAT3(0, 0, 0);
	light.Range = 10;
	light.Intensity = 2;

	lights.push_back(light);

	Light light2 = Light();
	light2.Type = LIGHT_TYPE_POINT;
	light2.Color = XMFLOAT3(0, 0, 1);
	light2.Position = XMFLOAT3(-9, 0, 0);
	light2.Range = 10;
	light2.Intensity = 2;

	lights.push_back(light2);

	Light light4 = light3;
	light4.Direction = XMFLOAT3(-1, 0, 0);
	light4.Color = XMFLOAT3(1, 1, 0);

	lights.push_back(light4);

	Light light5 = light3;
	light5.Direction = XMFLOAT3(0, 0, -1);
	light5.Color = XMFLOAT3(0, 1, 1);

	lights.push_back(light5);
#pragma endregion
}

// --------------------------------------------------------
// Clean up memory or objects created by this class
// 
// Note: Using smart pointers means there probably won't
//       be much to manually clean up here!
// --------------------------------------------------------
Game::~Game()
{
	// Wait for the GPU before we shut down
	Graphics::WaitForGPU();
}

// --------------------------------------------------------
// Creates the geometry we're going to draw
// --------------------------------------------------------
void Game::CreateGeometry()
{
	cube = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/cube.igme540obj").c_str(), Graphics::Device);
	cylinder = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/cylinder.igme540obj").c_str(), Graphics::Device);
	helix = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/helix.igme540obj").c_str(), Graphics::Device);
	sphere = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/sphere.igme540obj").c_str(), Graphics::Device);
	torus = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/torus.igme540obj").c_str(), Graphics::Device);
	quad = std::make_shared<Mesh>(FixPath(L"../../Assets/Models/quad.igme540obj").c_str(), Graphics::Device);

	entities.push_back(std::make_shared<GameEntity>(cube, cobblestone));
	entities.push_back(std::make_shared<GameEntity>(cylinder, scratched));
	entities.push_back(std::make_shared<GameEntity>(helix, bronze));
	entities.push_back(std::make_shared<GameEntity>(sphere, floor));
	entities.push_back(std::make_shared<GameEntity>(torus, paint));
	entities.push_back(std::make_shared<GameEntity>(quad, iron));

	entities.push_back(std::make_shared<GameEntity>(cube, reflective));


	entities[0]->GetTransform()->SetPosition(-9, 0, 0);
	entities[1]->GetTransform()->SetPosition(-6, 0, 0);
	entities[2]->GetTransform()->SetPosition(-3, 0, 0);
	entities[4]->GetTransform()->SetPosition(3, 0, 0);
	entities[5]->GetTransform()->SetPosition(6, 0, 0);

	entities[6]->GetTransform()->SetPosition(0, -4, 0);
	entities[6]->GetTransform()->SetScale(6, 1, 6);
}


// --------------------------------------------------------
// Handle resizing to match the new window size
//  - Eventually, we'll want to update our 3D camera
// --------------------------------------------------------
void Game::OnResize()
{
	// Resize the viewport and scissor rectangle
	{
		// Set up the viewport so we render into the correct
		// portion of the render target
		viewport = {};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)Window::Width();
		viewport.Height = (float)Window::Height();
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		// Define a scissor rectangle that defines a portion of
		// the render target for clipping. This is different from
		// a viewport in that it is applied after the pixel shader.
		// We need at least one of these, but we're rendering to
		// the entire window, so it'll be the same size.
		scissorRect = {};
		scissorRect.left = 0;
		scissorRect.top = 0;
		scissorRect.right = Window::Width();
		scissorRect.bottom = Window::Height();

		if (camera.get() != NULL)
			camera.get()->UpdateProjectionMatrix(viewport.Width / viewport.Height);

		// Resize raytracing output texture
		RayTracing::ResizeOutputUAV(Window::Width(), Window::Height());
	}
}


// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::KeyDown(VK_ESCAPE))
		Window::Quit();

	camera.get()->Update(deltaTime);

	entities[0]->GetTransform()->Rotate(deltaTime, 0, 0);
	entities[5]->GetTransform()->Rotate(0, deltaTime, 0);

	entities[1]->GetTransform()->MoveAbsolute(0, deltaTime * cosf(totalTime), 0);
	entities[2]->GetTransform()->MoveAbsolute(0, deltaTime * -cosf(-totalTime), 0);
	entities[3]->GetTransform()->MoveAbsolute(0,0,deltaTime * cosf(totalTime));
	entities[4]->GetTransform()->MoveAbsolute(0,0,deltaTime * -cosf(-totalTime));
}


// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Grab the current back buffer for this frame
	Microsoft::WRL::ComPtr<ID3D12Resource> currentBackBuffer = Graphics::BackBuffers[Graphics::SwapChainIndex()];
	RayTracing::CreateTopLevelAccelerationStructureForScene(entities);

	// Rendering here!
	{
		RayTracing::Raytrace(camera, currentBackBuffer);
	}

	// Present
	{		
		// Must occur BEFORE present
		Graphics::CloseAndExecuteCommandList();
		
		// Present the current back buffer and move to the next one
		bool vsync = Graphics::VsyncState();
		Graphics::SwapChain->Present(vsync ? 1 : 0, vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);
		Graphics::AdvanceSwapChainIndex();
		
		// Wait for the GPU to be done and then reset the command list & allocator
		Graphics::WaitForGPU();
		Graphics::ResetAllocatorAndCommandList();
	}
}



