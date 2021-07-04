#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeMaterial;
	class HeCamera;
	class HeVertexArrayObject;
	template <typename T> class HeVertexBufferObject;

	class HeGeometry : public HeObject
	{
	public:
		enum FillMode { Fill, Wireframe };

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

		virtual void PreDraw(HeCamera* camera);
		virtual void Draw(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		virtual void PostDraw(HeCamera* camera);

		inline void SetHeMaterial(HeMaterial* material) { this->material = material; }
		inline void SetFillMode(FillMode fillMode) { this->fillMode = fillMode; }

	protected:
		bool dirty = true;

		FillMode fillMode = FillMode::Fill;
		GLenum drawingMode = GL_TRIANGLES;

		HeMaterial* material = nullptr;
		HeVertexArrayObject* vao;
		HeVertexBufferObject<glm::vec3>* vbo;
		HeVertexBufferObject<GLuint>* ibo;
		HeVertexBufferObject<glm::vec4>* cbo;
		HeVertexBufferObject<glm::vec2>* uvbo;
	};

}
