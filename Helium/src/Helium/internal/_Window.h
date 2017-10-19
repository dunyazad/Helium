#pragma once

#include <Helium/Helium.h>
#include <Helium/Window.h>

namespace ArtificialNature {

	class GraphicsDevice;

	class _Window : public Window
	{
	public:
		_Window(HWND hWnd);
		~_Window();

		static Window* CreateHeliumWindow(HWND hWnd);
		static int GetWindowCreateRequestCount();
		static const map<HWND, Window*>& GetWindowTable();

		static LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		virtual LRESULT InstanceWndProc(UINT message, WPARAM wParam, LPARAM lParam);

		virtual inline HWND GetWindowHandle() { return m_hWnd; }
		virtual void Frame();

	private:
		static map<HWND, Window*> s_windowTable;
		static int s_windowCreateRequestCount;
		static map<HWND, Window*> s_toDelete;

		HWND m_hWnd = nullptr;
		thread m_thread;
		bool m_finished = false;

		int m_width;
		int m_height;

		GraphicsDevice* m_pGraphicsDevice = nullptr;

		void CreateNewWindow();
	};

}
