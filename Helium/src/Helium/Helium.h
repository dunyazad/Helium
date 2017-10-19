#pragma once

#include <Helium/HeliumCommon.h>
#include <Helium/Window.h>

namespace ArtificialNature {

	class HELIUM_API Helium
	{
	public:
		virtual void ShowDebugConsoleWindow() = 0;
		virtual Window* CreateHeliumWindow(HWND hWnd) = 0;
		virtual void Run() = 0;
	};

}

extern "C" HELIUM_API ArtificialNature::Helium* CreateInstance();
