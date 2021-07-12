#include <Helium/Graphics/HeFrameBufferObject.h>

#include <Helium/Graphics/HeGraphics.h>
#include <Helium/Graphics/Texture/Texture.h>

namespace ArtificialNature {

	int count = 0;

	HeFrameBufferObject::HeFrameBufferObject(HeGraphics* graphics, int width, int height)
		: graphics(graphics), width(width), height(height)
	{

	}

	HeFrameBufferObject::HeFrameBufferObject(HeGraphics* graphics, HeTexture* texture)
		: graphics(graphics), targetTexture(texture)
	{
		if (targetTexture != nullptr)
		{
			width = targetTexture->GetWidth();
			height = targetTexture->GetHeight();
		}
	}

	HeFrameBufferObject::~HeFrameBufferObject()
	{
	}

	void HeFrameBufferObject::Initialize()
	{
		if (targetTexture == nullptr)
		{
			stringstream ss;
			ss << "FrameBufferObject Texture " << count;
			targetTexture = graphics->GetTexture(ss.str(), width, height);
		}
		

	}

	void HeFrameBufferObject::Terminate()
	{
	}

	void HeFrameBufferObject::Bind()
	{

	}

	void HeFrameBufferObject::Unbind()
	{

	}
}