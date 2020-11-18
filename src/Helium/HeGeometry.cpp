#include "HeGeometry.h"
#include "HeMaterial.h"
#include "HeCamera.h"

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

		vbo.Initialize(0);

		ibo.Initialize(-1);

		cbo.Initialize(1);

		uvbo.Initialize(2);

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

		dirty = true;
	}

	void HeGeometry::SetVertex(int index, const glm::vec3& vertex)
	{
		if(vbo.SetElement(index, vertex) == true)
			dirty = true;
	}

	void HeGeometry::AddIndex(GLuint index)
	{
		ibo.AddElement(index);

		dirty = true;
	}

	void HeGeometry::SetIndex(int index, GLuint vertexIndex)
	{
		if (ibo.SetElement(index, vertexIndex) == true)
			dirty = true;
	}

	void HeGeometry::AddColor(const glm::vec4& color)
	{
		cbo.AddElement(color);

		dirty = true;
	}

	void HeGeometry::SetColor(int index, const glm::vec4& color)
	{
		if (cbo.SetElement(index, color) == true)
			dirty = true;
	}

	void HeGeometry::AddUV(const glm::vec2& uv)
	{
		uvbo.AddElement(uv);

		dirty = true;
	}

	void HeGeometry::SetUV(int index, const glm::vec2& uv)
	{
		if (uvbo.SetElement(index, uv) == true)
			dirty = true;
	}

	void HeGeometry::Draw(HeCamera* camera)
	{
		if (material == nullptr)
			return;

		material->Use(camera);

		vao.Bind();

		if(vbo.Size() > 0) vbo.Bind();
		if(ibo.Size() > 0) ibo.Bind();
		if(cbo.Size() > 0) cbo.Bind();
		if(uvbo.Size() > 0) uvbo.Bind();

		CheckGLError();

		if (drawingMode == GL_LINES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glDrawElements(drawingMode, (GLsizei)ibo.Size(), GL_UNSIGNED_INT, 0);

		if (drawingMode == GL_LINES)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		CheckGLError();

		vao.Unbind();
	}
}
