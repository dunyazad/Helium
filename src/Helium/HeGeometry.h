#pragma once

#include "HeliumCommon.h"
#include "HeObject.h"

#include "HeVertexArrayObject.h"
#include "HeVertexBufferObject.hpp"
#include "HeIndexBufferObject.h"

namespace ArtificialNature {

	class HeMaterial;

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
		void AddIndex(GLuint index);
		void AddColor(const glm::vec4& color);
		void AddUV(const glm::vec2& uv);

		void Upload();

		void Draw();

		inline void SetHeMaterial(HeMaterial* material) { this->material = material; }
		inline void SetDrawingMode(DrawingMode drawingMode) { this->drawingMode = drawingMode; }

	protected:
		DrawingMode drawingMode = DrawingMode::Triangles;

		HeMaterial* material = nullptr;
		HeVertexArrayObject vao;
		HeVertexBufferObject<glm::vec3> vbo;
		HeVertexBufferObject<GLuint> ibo;
		HeVertexBufferObject<glm::vec4> cbo;
		HeVertexBufferObject<glm::vec2> uvbo;
	};

}
