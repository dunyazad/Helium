#include "HeIndexBufferObject.h"

namespace ArtificialNature {

	HeIndexBufferObject::HeIndexBufferObject()
	{
		glGenBuffers(1, &id);
	}

	HeIndexBufferObject::~HeIndexBufferObject()
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