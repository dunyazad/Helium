#include <Helium/Graphics/Texture/HeCanvasTexture.h>

#include <Helium/Graphics/Image/Image.h>

namespace ArtificialNature {

	HeCanvasTexture::HeCanvasTexture(const string& name, HeImage* image)
		: HeTexture(name, image)
	{
	}

	HeCanvasTexture::~HeCanvasTexture()
	{
	}

	void HeCanvasTexture::Initialize()
	{
		HeTexture::Initialize();
	}
	
}
