#pragma once
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <d3d12.h>
#include "Vertex.h"

class Mesh
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;
	D3D12_INDEX_BUFFER_VIEW ibView{};
	unsigned int indexCount;

	void CreateBuffers(Vertex* vertices, int vertexNum, unsigned int* indices, Microsoft::WRL::ComPtr<ID3D12Device> device);
public:
	Mesh();
	Mesh(const wchar_t* filename, Microsoft::WRL::ComPtr<ID3D12Device> device);
	Mesh(Vertex vertices[], unsigned int vertexNum, unsigned int indices[], unsigned int indexNum, Microsoft::WRL::ComPtr<ID3D12Device> device);
	~Mesh();
	void Draw();

	Microsoft::WRL::ComPtr<ID3D12Resource> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D12Resource> GetIndexBuffer();
	void CalculateTangents(Vertex* verts, int numVerts, unsigned int* indices, int numIndices);
	unsigned int GetIndexCount();
};

