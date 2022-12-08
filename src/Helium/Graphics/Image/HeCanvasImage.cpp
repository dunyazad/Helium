#include <Helium/Graphics/Image/HeCanvasImage.h>

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
		if (data != nullptr)
		{
			delete data;
		}
	}
	
	void HeCanvasImage::Initialize()
	{
	}

	void HeCanvasImage::CaptureFrame(const string& filepath, bool verticalFlip)
	{
		if (data != nullptr)
		{
			delete data;
		}

		data = new unsigned char[this->width * this->height * this->nrChannels];
		memset(data, 0, this->width * this->height * this->nrChannels);

		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

		stbi_flip_vertically_on_write(verticalFlip);
		stbi_write_png(filepath.c_str(), this->width, this->height, this->nrChannels, this->data, this->width * this->nrChannels);
	}
}
