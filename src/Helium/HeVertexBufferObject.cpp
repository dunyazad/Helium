#include "HeVertexBufferObject.h"

namespace ArtificialNature {

	HeVertexBufferObject::HeVertexBufferObject()
	{
		glGenBuffers(1, &id);
	}

	HeVertexBufferObject::~HeVertexBufferObject()
	{
		glDeleteBuffers(1, &id);
	}

	void HeVertexBufferObject::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}

	void HeVertexBufferObject::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}