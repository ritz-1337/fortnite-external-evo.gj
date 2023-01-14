#include "render.h"
#include <Windows.h>
#include <iostream>
#include "../cheat/esp/esp.h"
IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* p_Device = NULL;
D3DPRESENT_PARAMETERS p_Params = { NULL };

HWND GameHwnd = NULL;
RECT GameRect = { NULL };
HWND MyHwnd = NULL;
MSG Message = { NULL };

uintptr_t procid = NULL;



HRESULT Render::DirectXInit()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&p_Params, sizeof(p_Params));
	p_Params.Windowed = TRUE;
	p_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_Params.hDeviceWindow = MyHwnd;
	p_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_Params.BackBufferWidth = Width;
	p_Params.BackBufferHeight = Height;
	p_Params.EnableAutoDepthStencil = TRUE;
	p_Params.AutoDepthStencilFormat = D3DFMT_D16;
	p_Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, MyHwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_Params, 0, &p_Device)))
	{
		p_Object->Release();
		exit(4);
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(MyHwnd);
	ImGui_ImplDX9_Init(p_Device);
	ImGui::StyleColorsClassic();
	ImGuiStyle* style = &ImGui::GetStyle();


	ImGui::StyleColorsClassic();
	style->WindowPadding = ImVec2(8, 8);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(4, 2);
	style->FrameRounding = 0.0f;
	style->ItemSpacing = ImVec2(8, 4);
	style->ItemInnerSpacing = ImVec2(4, 4);
	style->IndentSpacing = 21.0f;
	style->ScrollbarSize = 14.0f;
	style->ScrollbarRounding = 0.0f;
	style->GrabMinSize = 10.0f;
	style->GrabRounding = 0.0f;
	style->TabRounding = 0.f;
	style->ChildRounding = 0.0f;
	style->WindowBorderSize = 1.f;
	style->ChildBorderSize = 1.f;
	style->PopupBorderSize = 0.f;
	style->FrameBorderSize = 0.f;
	style->TabBorderSize = 0.f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.000f, 0.678f, 0.929f, 1.0f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.0f, 0.0263f, 0.0357f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.059f, 0.051f, 0.071f, 1.00f);
	style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.071f, 0.071f, 0.090f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.0f, 0.0263f, 0.0357f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImColor(0.000f, 0.678f, 0.929f, 1.0f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0263f, 0.0357f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.263f, 0.357f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImColor(87, 119, 134, 255);
	style->Colors[ImGuiCol_SliderGrab] = ImColor(119, 134, 169, 150);
	style->Colors[ImGuiCol_SliderGrabActive] = ImColor(119, 134, 169, 150);
	style->Colors[ImGuiCol_Button] = ImColor(26, 23, 31, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(0.102f, 0.090f, 0.122f, 1.000f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_Column] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	style->WindowTitleAlign.x = 0.50f;
	style->FrameRounding = 0.0f;

	p_Object->Release();
	return S_OK;
}

void Render::CreateOverlay()
{
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		nullptr,
		LoadIcon(nullptr, IDI_APPLICATION),
		LoadCursor(nullptr, IDC_ARROW),
		nullptr,
		nullptr,
		("Magical Fortnite Adventures"),
		LoadIcon(nullptr, IDI_APPLICATION)
	};

	RECT Rect;
	GetWindowRect(GetDesktopWindow(), &Rect);

	RegisterClassExA(&wcex);

	MyHwnd = CreateWindowExA(NULL, ("Magical Fortnite Adventures"), ("On The Hub"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);


	SetWindowLong(MyHwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(MyHwnd, &margin);
	ShowWindow(MyHwnd, SW_SHOW);
	SetWindowPos(MyHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetLayeredWindowAttributes(MyHwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	UpdateWindow(MyHwnd);
}

void Render::CleanuoD3D()
{
	if (p_Device != NULL)
	{
		p_Device->EndScene();
		p_Device->Release();
	}
	if (p_Object != NULL)
	{
		p_Object->Release();
	}
}

void Render::render() {
	char fpsinfo[64];
	sprintf(fpsinfo, ("FPS: %03d"), Util::get_fps());
	ImGui::GetOverlayDrawList()->AddText(ImGui::GetFont(), 15, ImVec2(50, 50), ImColor(0, 255, 0), fpsinfo);
	
	//ImGui::GetOverlayDrawList()->AddRect(ImVec2((1920 / 2) - 100, (1080 / 2) - 100), ImVec2((1920 / 2) + 100, (1080 / 2) + 100), ImColor(0, 255, 0), 0, 0, 3);
	Esp::ActorLoop();
	if (GetAsyncKeyState(VK_RBUTTON) && bAimbot) {
		bIsTargeting = true;
		aimbot();
	}
	else {
		bIsTargeting = false;
	}
}

WPARAM Render::MainLoop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, MyHwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();
		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		if (hwnd_active == GameHwnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(MyHwnd, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(GameHwnd, &rc);
		ClientToScreen(GameHwnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.ImeWindowHandle = GameHwnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (GetAsyncKeyState(0x1)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{

			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			p_Params.BackBufferWidth = Width;
			p_Params.BackBufferHeight = Height;
			SetWindowPos(MyHwnd, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			p_Device->Reset(&p_Params);
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//do shit
		render();
		Menu();

		ImGui::EndFrame();
		p_Device->SetRenderState(D3DRS_ZENABLE, false);
		p_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		p_Device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		p_Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (p_Device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			p_Device->EndScene();
		}
		HRESULT result = p_Device->Present(NULL, NULL, NULL, NULL);

		if (result == D3DERR_DEVICELOST && p_Device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_Device->Reset(&p_Params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanuoD3D();
	DestroyWindow(MyHwnd);

	return Message.wParam;
}









































































































































































































bool bMenu = true;
//secret menu down here xD
void Render::Menu() {
	static int MenuTab = 0;
	float
		TextSpaceLine = 90.f,
		SpaceLineOne = 120.f,
		SpaceLineTwo = 280.f,
		SpaceLineThr = 420.f;
	static const char* HitboxList[]{ "Head","Neck","Chest","Pelvis" };
	static int SelectedHitbox = 0;

	static const char* MouseKeys[]{ "RMouse","LMouse","Control","Shift","Alt" };
	static int KeySelected = 0;
	ImGuiStyle* style = &ImGui::GetStyle();

	if (GetAsyncKeyState(VK_INSERT) & 1) bMenu = !bMenu;
	if (bMenu)
	{
		static POINT Mouse;
		GetCursorPos(&Mouse);
		ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(Mouse.x, Mouse.y), float(4), ImColor(255,0,0), 50);

		ImGui::SetNextWindowSize({ 620.f,350.f });

		ImGui::Begin("been ud for 6 months", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
		ImGui::SetCursorPos({ 36.f,31.f });
		ImGui::Text("Evotoogood");
		ImGui::SetCursorPos({ 22.f,56.f });
		if (ImGui::Button("Aimbot", { 89.f, 32.f }))
		{
			MenuTab = 0;
		}
		ImGui::SetCursorPos({ 22.f,93.f });
		if (ImGui::Button("Visuals", { 89.f, 32.f }))
		{
			MenuTab = 1;
		}
		ImGui::SetCursorPos({ 22.f,130.f });
		if (ImGui::Button("MISC", { 89.f, 32.f }))
		{
			MenuTab = 2;
		}
		ImGui::SetCursorPos({ 22.f,204.f });
		if (ImGui::Button("Discord", { 89.f, 32.f }))
		{
			system("start https://discord.gg/8jTAstg4GK");
		}
		ImGui::SetCursorPos({ 22.f,291.f });
		if (ImGui::Button("unload", { 65.f, 20.f }))
		{
			exit(0);
		}

		style->ItemSpacing = ImVec2(8, 8);

		if (MenuTab == 0)
		{
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild("##Aimbot", { 450.f,279.f }, true);
			ImGui::SetCursorPos({ 19.f,14.f });
			ImGui::Text("Evo-Aim:");
			ImGui::Checkbox("Aimbot", &bAimbot);
			ImGui::SliderFloat("Smooth", &Smooth, 2, 15);
			ImGui::SliderInt("Fov Size", &FovSize, 50, 600);
			
			
		}
		if (MenuTab == 1)
		{
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild("##Visuals", { 450.f,279.f }, true);
			ImGui::SetCursorPos({ 19.f,14.f });
			ImGui::Text("Evo-Enemy:");
			ImGui::Checkbox("Corner Box", &bCornerBox);
			
			
			
		}
		if (MenuTab == 2)
		{
			ImGui::SetCursorPos({ 137.f,39.f });
			ImGui::BeginChild("##Misc", { 450.f,279.f }, true);
			ImGui::SetCursorPos({ 19.f,14.f });
			ImGui::Text("Evo-Gay:");
		}
		ImGui::EndChild();
		ImGui::End();
	}
}
