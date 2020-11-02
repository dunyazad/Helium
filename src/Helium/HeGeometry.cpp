#include "HeGeometry.h"

namespace ArtificialNature {

	HeGeometry::HeGeometry()
	{
	}

	HeGeometry::~HeGeometry()
	{
	}

	void HeGeometry::Initialize()
	{
		vao.Initialize();
		vao.Bind();

		vbo.Initialize();

		ibo.Initialize();
		vao.Unbind();
	}

	void HeGeometry::Terminate()
	{
		vao.Bind();

		vbo.Terminate();

		ibo.Terminate();
		vao.Unbind();

		vao.Terminate();
	}

	void HeGeometry::Draw()
	{
		vao.Bind();

		glDrawElements(GL_TRIANGLES, ibo.Size(), GL_UNSIGNED_INT, 0);

		vao.Unbind();
	}
}
