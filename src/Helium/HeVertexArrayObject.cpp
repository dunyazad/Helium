#include "HeVertexArrayObject.h"

namespace ArtificialNature {

	HeVertexArrayObject::HeVertexArrayObject()
	{
		glGenVertexArrays(1, &id);
	}

	HeVertexArrayObject::~HeVertexArrayObject()
	{
		glDeleteVertexArrays(1, &id);
	}

	void HeVertexArrayObject::Bind()
	{
		glBindVertexArray(id);
	}

	void HeVertexArrayObject::Unbind()
	{
		glBindVertexArray(0);
	}
}
