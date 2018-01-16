#pragma once

#include <Helium/GraphicsDevice.h>

namespace ArtificialNature {

	class Window;

	class _GraphicsDevice : public GraphicsDevice
	{
	public:
		_GraphicsDevice(Window* pWindow);
		~_GraphicsDevice();

		virtual HRESULT Initialize(HWND hWnd);
		virtual void Terminate();

		virtual void Clear(float r = 0.0f, float g = 0.125f, float b = 0.3f, float a = 1.0f);
		virtual void Present();

		virtual void Resize(int width, int height);

	private:
		Window* m_pWindow = nullptr;
		HWND	m_hWnd;
		HDC		m_hDC;
		HGLRC	m_hRC;

		int m_width;
		int m_height;
	};
}