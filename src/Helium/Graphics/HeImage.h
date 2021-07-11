#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	class HeImage : public HeObject
	{
	public:
		HeImage(const string& name, const string& filePath);
		~HeImage();

		inline int Width() { return width; }
		inline int Height() { return height; }
		inline int Channels() { return nrChannels; }

		const unsigned char* Data() { return data; }

	protected:
		string filePath;

		int width;
		int height;
		int nrChannels;

		unsigned char* data;
	};

}
