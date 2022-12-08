#include <Helium/Graphics/Image/Image.h>

#include <stb/stb_image.h>

namespace ArtificialNature {

	HeImage::HeImage(const string& name, const string& filename, bool verticalFlip)
		: HeObject(name), filename(filename), verticalFlip(verticalFlip)
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
		data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	}

	void HeImage::Write(const string& outputFilename, bool verticalFlip)
	{
		stbi_flip_vertically_on_write(verticalFlip);
		stbi_write_png(outputFilename.c_str(), this->width, this->height, this->nrChannels, this->data, this->width * this->nrChannels);
	}
}
