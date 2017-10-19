#pragma once

#include <Helium\HeliumCommon.h>

namespace ArtificialNature {

	class HELIUM_API GraphicsDevice
	{
	public:
		virtual HRESULT Initialize(HWND hWnd) = 0;
		virtual void Terminate() = 0;

		virtual void Clear(float r = 0.0f, float g = 0.125f, float b = 0.3f, float a = 1.0f) = 0;
		virtual void Present() = 0;

		virtual void Resize(int width, int height) = 0;
	};
}