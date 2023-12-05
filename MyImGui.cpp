#include "MyImGui.h"

MyImGui* MyImGui::GetInstance()
{
	static MyImGui instance;
	return &instance;
}

void MyImGui::Initialize(WindowsApp* win, DirectXCore* dxCore)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(win->GetHwnd());
	ImGui_ImplDX12_Init(dxCore->GetDevice(),
		dxCore->GetSwapChainDesc().BufferCount,
		dxCore->GetRtvDesc().Format,
		dxCore->GetSrvDescriptorHeap(),
		dxCore->GetSrvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
		dxCore->GetSrvDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
}

void MyImGui::BeginFlame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MyImGui::EndFlame()
{
	ImGui::Render();
}
