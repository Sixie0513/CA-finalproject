#include "Frame.h"

Frame::Frame()
{
}

Frame::~Frame()
{
	ReleaseBuffer();
}

void Frame::AddCamera(Camera* pCamera)
{
	pCameraVec.push_back(pCamera);
}

void Frame::AddMesh(Mesh* pMesh)
{
	pMeshVec.push_back(pMesh);
}

void Frame::AddTexture(Texture* pTexture)
{
	pTextureVec.push_back(pTexture);
}

void Frame::AddRenderTexture(RenderTexture* pRenderTexture)
{
	pRenderTextureVec.push_back(pRenderTexture);
}

void Frame::SetUniformTime(uint32_t time)
{
	uniform.time = time;
}

uint32_t Frame::GetUniformTime()
{
	return uniform.time;
}

bool Frame::CreateUniformBuffer(ID3D12Device* device)
{

	HRESULT hr;
	CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_UPLOAD); // 將 CD3DX
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(FrameUniform)); // 將 CD3DX12_RESOURCE_DESC 轉換為左值

	hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE, // no flags
		&resourceDesc, // 資源描述
		D3D12_RESOURCE_STATE_GENERIC_READ, // will be data that is read from so we keep it in the generic read state
		nullptr, // we do not have use an optimized clear value for constant buffers
		IID_PPV_ARGS(&gpuUniformBuffer));

	if (FAILED(hr))
	{
		return false;
	}

	gpuUniformBuffer->SetName(L"frame uniform buffer");

	CD3DX12_RANGE readRange(0, 0);    // We do not intend to read from this resource on the CPU. (so end is less than or equal to begin)

	// map the resource heap to get a gpu virtual address to the beginning of the heap
	hr = gpuUniformBuffer->Map(0, &readRange, &cpuUniformBufferAddress);

	return true;
}

void Frame::UpdateUniformBuffer()
{
	memcpy(cpuUniformBufferAddress, &uniform, sizeof(uniform));
}

void Frame::ReleaseBuffer()
{
	SAFE_RELEASE(gpuUniformBuffer);
}

D3D12_GPU_VIRTUAL_ADDRESS Frame::GetUniformBufferGpuAddress()
{
	return gpuUniformBuffer->GetGPUVirtualAddress();
}

bool Frame::InitFrame(ID3D12Device* device)
{
	if (!CreateUniformBuffer(device))
		return false;
	UpdateUniformBuffer();
	return true;
}

vector<Texture*>& Frame::GetTextureVec()
{
	return pTextureVec;
}

vector<RenderTexture*>& Frame::GetRenderTextureVec()
{
	return pRenderTextureVec;
}

vector<Camera*>& Frame::GetCameraVec()
{
	return pCameraVec;
}

vector<Mesh*>& Frame::GetMeshVec()
{
	return pMeshVec;
}