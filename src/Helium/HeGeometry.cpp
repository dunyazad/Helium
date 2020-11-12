#include "HeGeometry.h"
#include "HeMaterial.h"

namespace ArtificialNature {

	HeGeometry::HeGeometry() :
		vbo(HeVertexBufferObject<glm::vec3>::BufferType::VERTEX_BUFFER),
		ibo(HeVertexBufferObject<GLuint>::BufferType::INDEX_BUFFER),
		cbo(HeVertexBufferObject<glm::vec4>::BufferType::COLOR_BUFFER),
		uvbo(HeVertexBufferObject<glm::vec2>::BufferType::UV_BUFFER)
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

		cbo.Initialize();

		uvbo.Initialize();

		vao.Unbind();
	}

	void HeGeometry::Terminate()
	{
		vao.Bind();

		vbo.Unbind();
		vbo.Terminate();

		ibo.Unbind();
		ibo.Terminate();

		cbo.Unbind();
		cbo.Terminate();

		uvbo.Unbind();
		uvbo.Terminate();

		vao.Unbind();
		vao.Terminate();
	}

	void HeGeometry::AddVertex(const glm::vec3& vertex)
	{
		vbo.AddElement(vertex);
	}

	void HeGeometry::AddIndex(GLuint index)
	{
		ibo.AddElement(index);
	}

	void HeGeometry::AddColor(const glm::vec4& color)
	{
		cbo.AddElement(color);
	}

	void HeGeometry::AddUV(const glm::vec2& uv)
	{
		uvbo.AddElement(uv);
	}

	void HeGeometry::Upload()
	{
		vao.Bind();
		
		vbo.Upload(0);

		ibo.Upload(-1);

		cbo.Upload(1);

		uvbo.Upload(2);

		vao.Unbind();
	}

	void HeGeometry::Draw()
	{
		if (material == nullptr)
			return;

		material->Use();

		vao.Bind();

		vbo.Bind();
		cbo.Bind();
		uvbo.Bind();

		CheckGLError();

		//glEnableVertexAttribArray(0);
		//glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);

		glDrawElements(drawingMode, (GLsizei)ibo.Size(), GL_UNSIGNED_INT, 0);

		CheckGLError();

		//glDrawArrays(GL_TRIANGLES, 0, 3);

		vao.Unbind();
	}
}
