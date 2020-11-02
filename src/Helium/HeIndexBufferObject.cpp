#include "HeIndexBufferObject.h"

namespace ArtificialNature {

	HeIndexBufferObject::HeIndexBufferObject()
	{
	}

	HeIndexBufferObject::~HeIndexBufferObject()
	{
	}

	void HeIndexBufferObject::Initialize()
	{
		glGenBuffers(1, &id);
	}

	void HeIndexBufferObject::Terminate()
	{
		glDeleteBuffers(1, &id);
	}

	void HeIndexBufferObject::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void HeIndexBufferObject::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}