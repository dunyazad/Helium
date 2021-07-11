#include <Helium/Graphics/Image/HeCanvasImage.h>

#include <stb/stb_image.h>

namespace ArtificialNature {

	HeCanvasImage::HeCanvasImage(const string& name, int width, int height)
		: HeImage(name, "")
	{
		this->width = width;
		this->height = height;
		this->nrChannels = 4;
	}

	HeCanvasImage::~HeCanvasImage()
	{
	}

	void HeCanvasImage::Initialize()
	{
	}

	void HeCanvasImage::Terminate()
	{
	}
}
