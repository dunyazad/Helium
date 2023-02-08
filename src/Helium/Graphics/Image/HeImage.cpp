#include <Helium/Graphics/Image/Image.h>
#include <Helium/Graphics/HeGraphics.h>

namespace ArtificialNature {

	HeImage::HeImage(HeGraphics* pGraphics, const string& name, const string& filename, bool verticalFlip)
		: HeObject(name), pGraphics(pGraphics), filename(filename), verticalFlip(verticalFlip)
	{
	}

	HeImage::~HeImage()
	{
		if (data != nullptr) {
			stbi_image_free(data);
		}
	}

	void HeImage::Initialize()
	{
		stbi_set_flip_vertically_on_load(verticalFlip);
		data = stbi_load(filename.c_str(), &width, &height, &nrChannels, bits);
	}

	void HeImage::Write(const string& outputFilename, bool verticalFlip)
	{
		stbi_flip_vertically_on_write(verticalFlip);
		stbi_write_png(outputFilename.c_str(), width, height, nrChannels, data, width * nrChannels);
	}

	HeImage* HeImage::ResizeToPOT(HeImage* from)
	{
		HeImage* result = from->pGraphics->GetImage(from->GetName() + "_resized", from->filename, from->verticalFlip);

		auto wpot = NextPowerOf2(from->width);
		auto hpot = NextPowerOf2(from->height);
		auto n = wpot > hpot ? wpot : hpot;

		auto newData = new unsigned char[n * n * from->nrChannels];
		memset(newData, 255, n * n * from->nrChannels);

		for (size_t h = 0; h < from->height; h++)
		{
			int targetIndex = h * n * from->nrChannels;
			int sourceIndex = h * from->width * from->nrChannels;
			memcpy(newData + targetIndex, from->data + sourceIndex, from->width * from->nrChannels);
		}

		if (result->data != nullptr) {
			delete result->data;
		}
		result->data = newData;
		result->width = n;
		result->height = n;
		result->nrChannels = from->nrChannels;

		result->resizedToPOT = true;
		result->potResizedRatioW = (float)n / (float)from->width;
		result->potResizedRatioH = (float)n / (float)from->height;

		return result;
	}
}
