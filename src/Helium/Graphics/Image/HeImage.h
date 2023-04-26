#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	class HeGraphics;

	class HeImage : public HeObject
	{
	public:
		inline int GetWidth() { return width; }
		inline int GetHeight() { return height; }
		inline int GetChannels() { return nrChannels; }

		unsigned char* Data() { return data; }

		virtual void Initialize();

		void Write(const string& outputFilename, bool verticalFlip = true);

		static HeImage* ResizeToPOT(HeImage* from);

	protected:
		HeImage(HeGraphics* pGraphics, const string& name, const string& filename, bool verticalFlip = true);
		~HeImage();

		HeGraphics* pGraphics = nullptr;

		string filename;

		int width = 0;
		int height = 0;
		int nrChannels = 0;
		int bits = 0;

		unsigned char* data = nullptr;

		bool verticalFlip = true;

		bool resizedToPOT = false;
		float potResizedRatioW = 1.0f;
		float potResizedRatioH = 1.0f;

	public:
		friend class HeGraphics;
	};

}
