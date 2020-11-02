#include "HeVertexBufferObject.h"

namespace ArtificialNature {

	HeVertexBufferObject::HeVertexBufferObject()
	{
	}

	HeVertexBufferObject::~HeVertexBufferObject()
	{
	}

	void HeVertexBufferObject::Initialize()
	{
		glGenBuffers(1, &id);
	}
	
	void HeVertexBufferObject::Terminate()
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