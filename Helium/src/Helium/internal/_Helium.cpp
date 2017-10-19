#include "stdafx.h"

#include <Helium/internal/_Helium.h>

ArtificialNature::Helium * CreateInstance()
{
	return new ArtificialNature::_Helium();
}

namespace ArtificialNature {
	
	class DebugConsoleWindow
	{
	public:
		DebugConsoleWindow()
		{
			::AllocConsole();
			FILE* stream;
			freopen_s(&stream, "CONOUT$", "wt", stdout);

			bool bDualMonitor = GetSystemMetrics(SM_CMONITORS) > 1;
			if (bDualMonitor == true)
			{
				HWND hConsoleWindow = GetConsoleWindow();
				const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
				RECT rect;
				GetWindowRect(hConsoleWindow, &rect);
				int windowX = rect.left;
				if (windowX > screenWidth)
				{
					windowX -= screenWidth;
				}
				else
				{
					windowX += screenWidth;
				}
				SetWindowPos(hConsoleWindow, NULL, windowX, rect.top, 0, 0, SWP_NOSIZE);
			}
		}

		~DebugConsoleWindow()
		{
			::FreeConsole();
		}

		static DebugConsoleWindow debugConsoleWindow;
	};


	
	
	_Helium::_Helium()
	{
	}

	_Helium::~_Helium()
	{
	}

	void _Helium::ShowDebugConsoleWindow()
	{
		m_pDebugConsoleWindow = new DebugConsoleWindow();
	}

	Window* _Helium::CreateHeliumWindow(HWND hWnd)
	{
		return Window::CreateHeliumWindow(hWnd);
	}

	void _Helium::Run()
	{
		while (0 < Window::GetWindowCreateRequestCount()) {}
		while (false == Window::GetWindowTable().empty()) {}
	}
}
