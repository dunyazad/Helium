#pragma once

#include <Helium/HeliumCommon.h>

namespace ArtificialNature {

	class Scene;

	class HELIUM_API Window
	{
	public:
		static Window* CreateHeliumWindow(HWND hWnd);
		static const map<HWND, Window*>& GetWindowTable();
		static int GetWindowCreateRequestCount();

		virtual LRESULT InstanceWndProc(UINT message, WPARAM wParam, LPARAM lParam) = 0;
		virtual HWND GetWindowHandle() = 0;
		virtual void Frame() = 0;

		virtual Scene* CreateScene(const string& name) = 0;
	};

}
