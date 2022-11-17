#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Core/HeObject.h>

#include <Helium/Graphics/Image/HeImage.h>

namespace ArtificialNature {

	class HeCanvasImage : public HeImage
	{
	public:
		virtual void Initialize();
		virtual void Terminate();

		void CaptureFrame(const string& filepath, bool verticalFlip = true);

	protected:
		HeCanvasImage(const string& name, int width, int height);
		~HeCanvasImage();

	public:
		friend class HeGraphics;
	};

}
