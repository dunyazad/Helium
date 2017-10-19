#include "stdafx.h"

#include <Helium\internal\_GraphicsDevice.h>
#include <Helium\Window.h>

namespace ArtificialNature {

	_GraphicsDevice::_GraphicsDevice(Window* pWindow) : m_pWindow(pWindow)
	{
		this->Initialize(pWindow->GetWindowHandle());
	}

	_GraphicsDevice::~_GraphicsDevice()
	{
	}

	HRESULT _GraphicsDevice::Initialize(HWND hWnd)
	{
		m_hWnd = hWnd;

		GLuint PixelFormat;
		static PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			32,											// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		if (!(m_hDC = GetDC(hWnd)))							// Did We Get A Device Context?
		{
			MessageBox(NULL, L"Can't Create A GL Device Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return S_FALSE;								// Return FALSE
		}

		if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			MessageBox(NULL, L"Can't Find A Suitable PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return S_FALSE;								// Return FALSE
		}

		if (!SetPixelFormat(m_hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
		{
			MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return S_FALSE;								// Return FALSE
		}

		if (!(m_hRC = wglCreateContext(m_hDC)))				// Are We Able To Get A Rendering Context?
		{
			MessageBox(NULL, L"Can't Create A GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return S_FALSE;								// Return FALSE
		}

		if (!wglMakeCurrent(m_hDC, m_hRC))					// Try To Activate The Rendering Context
		{
			MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return S_FALSE;								// Return FALSE
		}

		// Initialize GLEW
		GLenum result = glewInit();
		if (result != GLEW_OK)
		{
			printf("Failed to initialize GLEW\n");
			return S_FALSE;
		}

		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		//glCullFace(GL_FRONT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return S_OK;
	}

	void _GraphicsDevice::Terminate()
	{
		wglMakeCurrent(0, 0);
		wglDeleteContext(m_hRC);
		ReleaseDC(m_hWnd, m_hDC);
	}

	void _GraphicsDevice::Clear(float r, float g, float b, float a)
	{
		wglMakeCurrent(m_hDC, m_hRC);

		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void _GraphicsDevice::Present()
	{
		SwapBuffers(m_hDC);
	}

	void _GraphicsDevice::Resize(int width, int height)
	{

	}
}