#include <Windows.h>
#include <iostream>
#include "render/render.h"
#include "cheat/esp/esp.h"
auto main() -> int //memez
{
	//made by ritz with luv xxx

	driver::find_driver();
	ProcId = driver::find_process((L"FortniteClient-Win64-Shipping.exe"));
	BaseId = driver::find_image();
	if (Debug::PrintPointers) {
		Util::PrintPtr("ProcessId: ", ProcId);
		Util::PrintPtr("BaseId: ", BaseId);
	}

	Render::CreateOverlay();
	Render::DirectXInit();
	Render::MainLoop();
}
