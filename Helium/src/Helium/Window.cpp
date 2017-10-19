#include "stdafx.h"

#include <Helium/Window.h>
#include <Helium/internal/_Window.h>

namespace ArtificialNature {

	Window* Window::CreateHeliumWindow(HWND hWnd)
	{
		return _Window::CreateHeliumWindow(hWnd);
	}

	int Window::GetWindowCreateRequestCount()
	{
		return _Window::GetWindowCreateRequestCount();
	}

	const map<HWND, Window*>& Window::GetWindowTable()
	{
		return _Window::GetWindowTable();
	}
}
