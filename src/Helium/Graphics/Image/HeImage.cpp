#include <Helium/Graphics/Image/Image.h>

#include <stb/stb_image.h>

namespace ArtificialNature {

	HeImage::HeImage(const string& name, const string& filePath)
		: HeObject(name), filePath(filePath)
	{
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
	}

	HeImage::~HeImage()
	{
		stbi_image_free(data);
	}

}