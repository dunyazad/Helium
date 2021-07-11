#include <Helium/Graphics/Geometry/HeGeometry.h>

#include <Helium/Graphics/HeMaterial.h>
#include <Helium/Graphics/HeVertexArrayObject.h>
#include <Helium/Graphics/HeVertexBufferObject.hpp>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeGeometry::HeGeometry(const string& name)
		: HeObject(name)
	{
		vao = new HeVertexArrayObject();
		vbo = new HeVertexBufferObject<glm::vec3>(HeVertexBufferObject<glm::vec3>::BufferType::VERTEX_BUFFER);
		ibo = new HeVertexBufferObject<GLuint>(HeVertexBufferObject<GLuint>::BufferType::INDEX_BUFFER);
		cbo = new HeVertexBufferObject<glm::vec4>(HeVertexBufferObject<glm::vec4>::BufferType::COLOR_BUFFER);
		uvbo = new HeVertexBufferObject<glm::vec2>(HeVertexBufferObject<glm::vec2>::BufferType::UV_BUFFER);
	}

	HeGeometry::~HeGeometry()
	{
		HeDelete(vao);
		HeDelete(vbo);
		HeDelete(ibo);
		HeDelete(cbo);
		HeDelete(uvbo);
	}

	void HeGeometry::Initialize()
	{
		vao->Initialize();
		vao->Bind();

		vbo->Initialize(0);

		ibo->Initialize(-1);

		cbo->Initialize(1);

		uvbo->Initialize(2);

		vao->Unbind();
	}

	void HeGeometry::Terminate()
	{
		vao->Bind();

		vbo->Unbind();
		vbo->Terminate();

		ibo->Unbind();
		ibo->Terminate();

		cbo->Unbind();
		cbo->Terminate();

		uvbo->Unbind();
		uvbo->Terminate();

		vao->Unbind();
		vao->Terminate();
	}

	void HeGeometry::AddVertex(const glm::vec3& vertex)
	{
		vbo->AddElement(vertex);

		dirty = true;
	}

	void HeGeometry::SetVertex(int index, const glm::vec3& vertex)
	{
		if(vbo->SetElement(index, vertex) == true)
			dirty = true;
	}

	const glm::vec3& HeGeometry::GetVertex(int index)
	{
		return vbo->GetElement(index);
	}

	void HeGeometry::AddIndex(GLuint index)
	{
		ibo->AddElement(index);

		dirty = true;
	}

	void HeGeometry::SetIndex(int index, GLuint vertexIndex)
	{
		if (ibo->SetElement(index, vertexIndex) == true)
			dirty = true;
	}

	void HeGeometry::AddColor(const glm::vec4& color)
	{
		cbo->AddElement(color);

		dirty = true;
	}

	void HeGeometry::SetColor(int index, const glm::vec4& color)
	{
		if (cbo->SetElement(index, color) == true)
			dirty = true;
	}

	void HeGeometry::AddUV(const glm::vec2& uv)
	{
		uvbo->AddElement(uv);

		dirty = true;
	}

	void HeGeometry::SetUV(int index, const glm::vec2& uv)
	{
		if (uvbo->SetElement(index, uv) == true)
			dirty = true;
	}

	void HeGeometry::PreDraw(HeCamera* camera)
	{

	}

	void HeGeometry::Draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model)
	{
		if (material == nullptr)
			return;

		material->Use(projection, view, model);

		vao->Bind();

		if(vbo->Size() > 0) vbo->Bind();
		if(ibo->Size() > 0) ibo->Bind();
		if(cbo->Size() > 0) cbo->Bind();
		if(uvbo->Size() > 0) uvbo->Bind();

		CheckGLError();

		if (fillMode == Wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		if (ibo->Size() > 0)
		{
			glDrawElements(drawingMode, (GLsizei)ibo->Size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(drawingMode, 0, (GLsizei)vbo->Size());
		}

		if (fillMode == Wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		CheckGLError();

		material->StopUse();

		vao->Unbind();
	}

	void HeGeometry::PostDraw(HeCamera* camera)
	{

	}

}
