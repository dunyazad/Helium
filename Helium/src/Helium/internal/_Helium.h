#pragma once

#include <Helium/Helium.h>
#include <Helium/Window.h>

namespace ArtificialNature {

	class DebugConsoleWindow;

	class _Helium : public Helium
	{
	public:
		_Helium();
		~_Helium();

		virtual void ShowDebugConsoleWindow();
		virtual Window* CreateHeliumWindow(HWND hWnd);
		virtual void Run();

	private:
		DebugConsoleWindow* m_pDebugConsoleWindow;
	};

}