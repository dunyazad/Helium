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

		void Write(const string& outputFilename, bool verticalFlip = true);

	protected:
		HeImage(const string& name, const string& filename, bool verticalFlip = true);
		~HeImage();

		string filename;

		int width = 0;
		int height = 0;
		int nrChannels = 0;

		unsigned char* data = nullptr;

		bool verticalFlip = true;
	public:
		friend class HeGraphics;
	};

}
