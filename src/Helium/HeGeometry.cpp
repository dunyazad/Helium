#include "HeGeometry.h"
#include "HeShader.h"

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

		vbo.Unbind();
		vbo.Terminate();

		ibo.Unbind();
		ibo.Terminate();

		vao.Unbind();
		vao.Terminate();
	}

	void HeGeometry::AddVertex(const glm::vec3& vertex)
	{
		vbo.AddVertex(vertex);
	}

	void HeGeometry::AddIndex(unsigned int index)
	{
		ibo.AddIndex(index);
	}

	void HeGeometry::Upload()
	{
		vao.Bind();
		
		vbo.Bind();
		vbo.Upload();
		//vbo.Unbind();

		ibo.Bind();
		ibo.Upload();
		//ibo.Unbind();

		vao.Unbind();
	}

	void HeGeometry::Draw()
	{
		if (shader == nullptr)
			return;

		shader->Use();

		vao.Bind();

		glDrawElements(GL_TRIANGLES, (GLsizei)ibo.Size(), GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		vao.Unbind();
	}
}
