#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeMaterial;
	class HeCamera;
	class HeVertexArrayObject;
	template <typename T> class HeVertexBufferObject;
	class HeAABB;
	class HeOctreeGeometry;

	class HeGeometry : public HeObject
	{
	public:
		enum FillMode { Fill, Wireframe };
		enum DrawingMode { Points = GL_POINTS, Lines = GL_LINES, LineLoop = GL_LINE_LOOP, LineStrip = GL_LINE_STRIP, Triangles = GL_TRIANGLES, TriangleStrip = GL_TRIANGLE_STRIP, TriangleFan = GL_TRIANGLE_FAN, Quads = GL_QUADS };

	public:
		virtual void Initialize();
		virtual void Terminate();

		void AddVertex(const glm::vec3& vertex);
		void SetVertex(int index, const glm::vec3& vertex);
		void SetVertices(const vector<glm::vec3>& vertices);
		const glm::vec3& GetVertex(int index) const;
		size_t GetVertexCount();
		void ClearVertices();
		
		void AddNormal(const glm::vec3& normal);
		void SetNormal(int index, const glm::vec3& normal);
		void SetNormals(const vector<glm::vec3>& normals);
		const glm::vec3& GetNormal(int index) const;
		size_t GetNormalCount();
		void ClearNormals();

		void AddIndex(GLuint index);
		void SetIndex(int index, GLuint vertexIndex);
		GLuint GetIndex(int at) const;
		size_t GetIndexCount();
		void ClearIndices();

		void AddColor(const glm::vec4& color);
		void SetColor(int index, const glm::vec4& color);
		size_t GetColorsCount();
		void ClearColors();

		void AddUV(const glm::vec2& uv);
		void SetUV(int index, const glm::vec2& uv);
		void SetUVs(const vector<glm::vec2>& uvs);
		const glm::vec2& GetUV(int index) const;
		size_t GetUVCount();
		void ClearUVs();

		virtual vector<int> RayIntersect(float screenX, float screenY, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
		virtual vector<int> RayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);

		virtual vector<int> LineIntersect(const glm::vec3& p0, const glm::vec3& p1);

		virtual void PreDraw(HeCamera* camera);
		virtual void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
		virtual void PostDraw(HeCamera* camera);

		inline HeMaterial* GetMaterial() const { return material; }
		inline void SetMaterial(HeMaterial* material) { this->material = material; }

		inline FillMode GetFillMode() const { return fillMode; }
		inline void SetFillMode(FillMode fillMode) { this->fillMode = fillMode; }

		inline DrawingMode GetDrawingMode() const { return drawingMode; }
		inline void SetDrawingMode(DrawingMode drawingMode) { this->drawingMode = drawingMode; }

		inline const HeAABB* GetAABB() const { return aabb; }
		inline const HeOctreeGeometry* GetOctree() const { return octree; }

		inline size_t GetFaceCount() { return GetIndexCount() / 3; }

		inline HeVertexBufferObject<glm::vec3>* GetVBO() { return vbo; }
		inline HeVertexBufferObject<glm::vec3>* GetVNBO() { return vnbo; }
		inline HeVertexBufferObject<GLuint>* GetIBO() { return ibo; }
		inline HeVertexBufferObject<glm::vec4>* GetCBO() { return cbo; }
		inline HeVertexBufferObject<glm::vec2>* GetUVBO() { return uvbo; }

	protected:
		HeGeometry(const string& name);
		~HeGeometry();

		bool dirty = true;

		FillMode fillMode = FillMode::Fill;
		DrawingMode drawingMode = Triangles;

		HeMaterial* material = nullptr;
		HeVertexArrayObject* vao;
		HeVertexBufferObject<glm::vec3>* vbo;
		HeVertexBufferObject<glm::vec3>* vnbo;
		HeVertexBufferObject<GLuint>* ibo;
		HeVertexBufferObject<glm::vec4>* cbo;
		HeVertexBufferObject<glm::vec2>* uvbo;

		HeAABB* aabb;
		HeOctreeGeometry* octree;

		void RebuildOctree();

	public:
		friend class HeGraphics;
		friend class HeCUDAInterface;
	};

}
