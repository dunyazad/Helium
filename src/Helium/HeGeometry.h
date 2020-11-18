#pragma once

#include "HeliumCommon.h"
#include "HeObject.h"

#include "HeVertexArrayObject.h"
#include "HeVertexBufferObject.hpp"

namespace ArtificialNature {

	class HeMaterial;
	class HeCamera;

	class HeGeometry : public HeObject
	{
	public:
		enum DrawingMode { Points = GL_POINTS, Lines = GL_LINES, Triangles = GL_TRIANGLES, Quads = GL_QUADS };

	public:
		HeGeometry();
		~HeGeometry();

		void Initialize();
		void Terminate();

		void AddVertex(const glm::vec3& vertex);
		void SetVertex(int index, const glm::vec3& vertex);
		const glm::vec3& GetVertex(int index);

		void AddIndex(GLuint index);
		void SetIndex(int index, GLuint vertexIndex);

		void AddColor(const glm::vec4& color);
		void SetColor(int index, const glm::vec4& color);

		void AddUV(const glm::vec2& uv);
		void SetUV(int index, const glm::vec2& uv);

		void Draw(HeCamera* camera);

		inline void SetHeMaterial(HeMaterial* material) { this->material = material; }
		inline void SetDrawingMode(DrawingMode drawingMode) { this->drawingMode = drawingMode; }

	protected:
		bool dirty = true;

		DrawingMode drawingMode = DrawingMode::Triangles;

		HeMaterial* material = nullptr;
		HeVertexArrayObject vao;
		HeVertexBufferObject<glm::vec3> vbo;
		HeVertexBufferObject<GLuint> ibo;
		HeVertexBufferObject<glm::vec4> cbo;
		HeVertexBufferObject<glm::vec2> uvbo;
	};

}
