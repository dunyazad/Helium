#include <Helium/Graphics/HeFrameBufferObject.h>

#include <Helium/Graphics/HeGraphics.h>
#include <Helium/Graphics/Texture/Texture.h>

namespace ArtificialNature {

	int count = 0;

	HeFrameBufferObject::HeFrameBufferObject(const string& name, HeGraphics* graphics, int width, int height)
		: HeObject(name), graphics(graphics), width(width), height(height)
	{

	}

	HeFrameBufferObject::HeFrameBufferObject(const string& name, HeGraphics* graphics, HeTexture* texture)
		: HeObject(name), graphics(graphics), targetTexture(texture)
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
		glGenFramebuffers(1, &fboID);
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);

		if (targetTexture == nullptr)
		{
			stringstream ss;
			ss << "FrameBufferObject Texture " << count++;
			targetTexture = graphics->GetTexture(ss.str(), width, height);
			targetTexture->Initialize();
		}


		targetTexture->Bind();
		glGenerateMipmap(GL_TEXTURE_2D);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture->GetTextureID(), 0);

		glGenRenderbuffers(1, &depthBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferID);

		//GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		//if (status == GL_FRAMEBUFFER_COMPLETE)
		//{
		//	printf("Framebuffer OK\n");
		//}
		//else
		//{
		//	printf("Framebuffer NOT OK\n");
		//}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void HeFrameBufferObject::Terminate()
	{
		if (depthBufferID != -1)
		{
			glDeleteRenderbuffers(1, &depthBufferID);
		}
		
		if (fboID != -1)
		{
			glDeleteFramebuffers(1, &fboID);
		}
	}

	void HeFrameBufferObject::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		targetTexture->Bind();
	}

	void HeFrameBufferObject::Unbind()
	{
		targetTexture->Unbind();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void HeFrameBufferObject::Resize(int width, int height)
	{
		this->width = width;
		this->height = height;

		targetTexture->Resize(width, height);
		
		if (depthBufferID != -1)
		{
			glDeleteRenderbuffers(1, &depthBufferID);
		}

		if (fboID != -1)
		{
			glDeleteFramebuffers(1, &fboID);
		}

		Initialize();
	}
}