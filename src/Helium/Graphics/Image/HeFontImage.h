#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Core/HeObject.h>

#include <Helium/Graphics/Image/HeImage.h>

namespace ArtificialNature {

	class HeGraphics;

	class HeFontImage : public HeImage
	{
	public:
		virtual void Initialize();

	protected:
		HeFontImage(HeGraphics* pGraphics, const string& name, const string& fontFilePath);
		~HeFontImage();

		string fontFilePath;

	public:
		friend class HeGraphics;
	};

}
