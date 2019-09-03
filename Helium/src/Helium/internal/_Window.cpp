#include "stdafx.h"

#include <Helium/internal/_Window.h>
#include <Helium/internal/_Scene.h>
#include <Helium/internal/_GraphicsDevice.h>

namespace ArtificialNature {

	map<HWND, Window*> _Window::s_windowTable;
	int _Window::s_windowCreateRequestCount = 0;

	map<HWND, Window*> _Window::s_toDelete;

	_Window::_Window(HWND hWnd) : m_hWnd(hWnd)
	{

	}

	_Window::~_Window()
	{
	}

	Window* _Window::CreateHeliumWindow(HWND hWnd)
	{
		auto w = s_windowTable.find(hWnd);
		if (nullptr == hWnd)
		{
			auto window = new _Window(hWnd);
			window->CreateNewWindow();
			return window;
		}
		else
		{
			if (w == s_windowTable.end())
			{
				auto window = new _Window(hWnd);
				window->m_pGraphicsDevice = new _GraphicsDevice(window);
				return window;
			}
			else
			{
				return (*w).second;
			}
		}
	}

	int _Window::GetWindowCreateRequestCount()
	{
		return s_windowCreateRequestCount;
	}

	void _Window::CreateNewWindow()
	{
		s_windowCreateRequestCount++;

		m_thread = thread([&] {
			HINSTANCE hInstance = ::GetModuleHandle(nullptr);

			WNDCLASSEXW wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = (WNDPROC)(&_Window::WndProc);
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hIcon = nullptr;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = L"HeliumWindow";
			wcex.hIconSm = nullptr;

			RegisterClassExW(&wcex);

			m_hWnd = CreateWindow(L"HeliumWindow", L"Helium", WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

			ShowWindow(m_hWnd, SW_SHOW);
			UpdateWindow(m_hWnd);

			s_windowTable[m_hWnd] = this;
			s_windowCreateRequestCount--;

			this->m_pGraphicsDevice = new _GraphicsDevice(this);

			while (!m_finished)
			{
				MSG msg{ 0 };
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						m_finished = true;
						s_toDelete[m_hWnd] = this;
						m_hWnd = nullptr;
					}
					else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
				else
				{
					this->Frame();
				}
			}

			s_windowTable.erase(m_hWnd);
		});
	}

	const map<HWND, Window*>& _Window::GetWindowTable()
	{
		return s_windowTable;
	}

	LRESULT _Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		for (auto& kvp : s_toDelete)
		{
			delete kvp.second;
			s_windowTable.erase(kvp.first);
		}
		s_toDelete.clear();

		auto w = s_windowTable.find(hWnd);
		if (w != s_windowTable.end())
		{
			return ((_Window*)(*w).second)->InstanceWndProc(message, wParam, lParam);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);;
	}

	LRESULT _Window::InstanceWndProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		//cout << m_hWnd << endl;

		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			m_finished = true;
			s_toDelete[m_hWnd] = this;
			m_hWnd = nullptr;
			break;
		case WM_SIZE:
			RECT rt;
			GetClientRect(m_hWnd, &rt);
			m_width = rt.right - rt.left;
			m_height = rt.bottom - rt.top;
			break;
		default:
			return DefWindowProc(m_hWnd, message, wParam, lParam);
		}
		return 0;
	}

	void _Window::Frame()
	{
		if(m_pGraphicsDevice) m_pGraphicsDevice->Clear();

		for (auto& kvp : m_scenes)
		{
			if (kvp.second)
			{
				kvp.second->Render(m_pGraphicsDevice);
			}
		}

		if (m_pGraphicsDevice) m_pGraphicsDevice->Present();
	}

	Scene* _Window::CreateScene(const string& name)
	{
		if (m_scenes.count(name) != 0)
			return m_scenes[name];
	
		auto pScene = new _Scene;
		m_scenes[name] = pScene;

		return pScene;
	}
}
