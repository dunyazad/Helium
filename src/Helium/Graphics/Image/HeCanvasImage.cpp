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
		if (context != nullptr)
		{
			delete context;
			context = nullptr;
		}

		if (image != nullptr)
		{
			delete image;
			image = nullptr;
		}
	}

	void HeCanvasImage::Initialize()
	{
		image = new BLImage(width, height, BL_FORMAT_PRGB32);
		context = new BLContext(*image);

		data = new unsigned char[width * height * 4];

		// Clear the image.
		context->setCompOp(BL_COMP_OP_SRC_COPY);
		context->fillAll();

		// Fill some path.
		BLPath path;
		path.moveTo(26, 31);
		path.cubicTo(642, 132, 587, -136, 25, 464);
		path.cubicTo(882, 404, 144, 267, 27, 31);

		context->setCompOp(BL_COMP_OP_SRC_OVER);
		context->setFillStyle(BLRgba32(0xFFFFFFFF));
		context->fillPath(path);

		// Detach the rendering context from `img`.
		context->end();

		// Let's use some built-in codecs provided by Blend2D.
		BLImageCodec codec;
		codec.findByExtension("PNG");
		BLArray<uint8_t> result;
		result.resize(width * height * 4, 255);
		image->writeToData(result, codec);

		for (size_t i = 0; i < result.size(); i++)
		{
			data[i] = result[i];
		}
		//memcpy(data, &result.at(0), width * height * 4);
	}

	void HeCanvasImage::Terminate()
	{

	}
}
