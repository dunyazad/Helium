#include <Helium/Graphics/HeVertexArrayObject.h>

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

		CheckGLError();
	}

	void HeVertexArrayObject::Terminate()
	{
		glDeleteVertexArrays(1, &id);

		CheckGLError();
	}

	void HeVertexArrayObject::Bind()
	{
		glBindVertexArray(id);

		CheckGLError();
	}

	void HeVertexArrayObject::Unbind()
	{
		glBindVertexArray(0);

		CheckGLError();
	}
}
