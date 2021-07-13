#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	class HeImage : public HeObject
	{
	public:
		inline int GetWidth() { return width; }
		inline int GetHeight() { return height; }
		inline int GetChannels() { return nrChannels; }

		const unsigned char* Data() { return data; }

		virtual void Initialize();
		virtual void Terminate();

	protected:
		HeImage(const string& name, const string& filePath, bool verticalFlip = true);
		~HeImage();

		string filePath;

		int width;
		int height;
		int nrChannels;

		unsigned char* data;

		bool verticalFlip = true;
	public:
		friend class HeGraphics;
	};

}
