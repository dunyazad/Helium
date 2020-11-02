#include "HeVertexArrayObject.h"

namespace ArtificialNature {

	HeVertexArrayObject::HeVertexArrayObject()
	{
	}

	HeVertexArrayObject::~HeVertexArrayObject()
	{
	}

	void HeVertexArrayObject::Initialize()
	{
		glGenVertexArrays(1, &id);
	}

	void HeVertexArrayObject::Terminate()
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
