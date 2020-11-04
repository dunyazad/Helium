#include "HeImage.h"

#include <stb/stb_image.h>

namespace ArtificialNature {

	HeImage::HeImage(const string& filePath)
		: filePath(filePath)
	{
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	}

	HeImage::~HeImage()
	{
		stbi_image_free(data);
	}

}