#include <Helium/Graphics/Geometry/HeGeometry.h>

#include <Helium/Graphics/Material/Material.h>
#include <Helium/Graphics/HeVertexArrayObject.h>
#include <Helium/Graphics/HeVertexBufferObject.hpp>
#include <Helium/Graphics/HeAABB.h>
#include <Helium/Graphics/HeOctree.h>

#include <Helium/Scene/HeCamera.h>

#include <Helium/Graphics/Geometry/Geometry.h>

namespace ArtificialNature {

	HeGeometry::HeGeometry(const string& name)
		: HeObject(name)
	{
		vao = new HeVertexArrayObject();
		vbo = new HeVertexBufferObject<glm::vec3>(HeVertexBufferObject<glm::vec3>::BufferType::VERTEX_BUFFER);
		vnbo = new HeVertexBufferObject<glm::vec3>(HeVertexBufferObject<glm::vec3>::BufferType::NORMAL_BUFFER);
		ibo = new HeVertexBufferObject<GLuint>(HeVertexBufferObject<GLuint>::BufferType::INDEX_BUFFER);
		cbo = new HeVertexBufferObject<glm::vec4>(HeVertexBufferObject<glm::vec4>::BufferType::COLOR_BUFFER);
		uvbo = new HeVertexBufferObject<glm::vec2>(HeVertexBufferObject<glm::vec2>::BufferType::UV_BUFFER);

		aabb = new HeAABB();
		octree = new HeOctreeGeometry(this, aabb->GetMin(), aabb->GetMax(), 0);
	}

	HeGeometry::~HeGeometry()
	{
		HeDelete(vao);
		HeDelete(vbo);
		HeDelete(vnbo);
		HeDelete(ibo);
		HeDelete(cbo);
		HeDelete(uvbo);

		delete aabb;
	}

	void HeGeometry::Initialize()
	{
		vao->Initialize();
		vao->Bind();

		vbo->Initialize(0);

		vnbo->Initialize(3);

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

		vnbo->Unbind();
		vnbo->Terminate();

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

		aabb->Extend(vertex);

		dirty = true;
	}

	void HeGeometry::SetVertex(int index, const glm::vec3& vertex)
	{
		if (vbo->SetElement(index, vertex) == true) {
			aabb->Extend(vertex);
			dirty = true;
		}
	}

	void HeGeometry::SetVertices(const vector<glm::vec3>& vertices)
	{
		vbo->SetElements(vertices);
		for (auto& v : vertices)
		{
			aabb->Extend(v);
		}
	}

	const glm::vec3& HeGeometry::GetVertex(int index) const
	{
		return vbo->GetElement(index);
	}

	size_t HeGeometry::GetVertexCount()
	{
		return vbo->Size();
	}

	void HeGeometry::ClearVertices()
	{
		vbo->Clear();
	}

	void HeGeometry::AddNormal(const glm::vec3& normal)
	{
		vnbo->AddElement(normal);

		dirty = true;
	}

	void HeGeometry::SetNormal(int index, const glm::vec3& normal)
	{
		if (vnbo->SetElement(index, normal) == true)
			dirty = true;
	}

	void HeGeometry::SetNormals(const vector<glm::vec3>& normals)
	{
		vnbo->SetElements(normals);
	}

	const glm::vec3& HeGeometry::GetNormal(int index) const
	{
		return vnbo->GetElement(index);
	}

	size_t HeGeometry::GetNormalCount()
	{
		return vnbo->Size();
	}

	void HeGeometry::ClearNormals()
	{
		vnbo->Clear();
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

	GLuint HeGeometry::GetIndex(int at) const
	{
		return ibo->GetElement(at);
	}

	size_t HeGeometry::GetIndexCount()
	{
		return ibo->Size();
	}

	void HeGeometry::ClearIndices()
	{
		ibo->Clear();
	}

	void HeGeometry::AddColor(const HeColor& color)
	{
		cbo->AddElement(color);

		dirty = true;
	}

	void HeGeometry::SetColor(int index, const HeColor& color)
	{
		if (cbo->SetElement(index, color) == true)
			dirty = true;
	}

	size_t HeGeometry::GetColorsCount()
	{
		return cbo->Size();
	}

	void HeGeometry::ClearColors()
	{
		cbo->Clear();
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

	void HeGeometry::SetUVs(const vector<glm::vec2>& uvs)
	{
		uvbo->SetElements(uvs);

		dirty = true;
	}

	const glm::vec2& HeGeometry::GetUV(int index) const
	{
		return uvbo->GetElement(index);
	}

	size_t HeGeometry::GetUVCount()
	{
		return uvbo->Size();
	}

	void HeGeometry::ClearUVs()
	{
		uvbo->Clear();
	}

	vector<int> HeGeometry::RayIntersect(float screenX, float screenY, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix)
	{
		if (dirty == true) {
			RebuildOctree();
			dirty = false;
		}

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		float winX = (float)screenX;
		float winY = (float)viewport[3] - (float)screenY;

		auto u = winX / viewport[2] - 0.5f;
		auto v = winY / viewport[3] - 0.5f;

		auto pp = glm::unProject(glm::vec3(winX, winY, 1), glm::identity<glm::mat4>(), projectionMatrix * viewMatrix, glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3]));

		auto rayOrigin = glm::vec3(glm::inverse(viewMatrix)[3]);
		auto rayDirection = glm::normalize(pp - rayOrigin);

		return RayIntersect(rayOrigin, rayDirection);
	}

	vector<int> HeGeometry::RayIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
	{
		if (dirty == true) {
			RebuildOctree();
			dirty = false;
		}

		vector<int> pickedFaceIndices;

		vector<HeOctreeGeometry*> intersectingOctrees;
		octree->GetRayInersectingOctrees(rayOrigin, rayDirection, intersectingOctrees);

		if (intersectingOctrees.size() > 0)
		{
			vector<tuple<float, int>> unorderedPickedFaceIndices;
			for (auto& ot : intersectingOctrees)
			{
				for (auto& fi : ot->GetContainingFaceIndices())
				{
					auto vi0 = GetIndex((int)fi * 3);
					auto vi1 = GetIndex((int)fi * 3 + 1);
					auto vi2 = GetIndex((int)fi * 3 + 2);

					auto& v0 = GetVertex(vi0);
					auto& v1 = GetVertex(vi1);
					auto& v2 = GetVertex(vi2);

					glm::vec2 baricenter;
					float distance = 0.0f;
					if (glm::intersectRayTriangle(rayOrigin, rayDirection, v0, v1, v2, baricenter, distance))
					{
						if (distance > 0) {
							unorderedPickedFaceIndices.push_back(make_tuple(distance, (int)fi));
						}
					}
				}
			}

			struct PickedFacesLess {
				inline bool operator() (const tuple<float, int>& a, const tuple<float, int>& b) {
					return get<0>(a) < get<0>(b);
				}
			};

			sort(unorderedPickedFaceIndices.begin(), unorderedPickedFaceIndices.end(), PickedFacesLess());

			for (auto& t : unorderedPickedFaceIndices)
			{
				if (get<1>(t) > 0.0f)
				{
					pickedFaceIndices.push_back(get<1>(t));
				}
			}
		}
	
		return pickedFaceIndices;
	}

	vector<int> HeGeometry::LineIntersect(const glm::vec3& p0, const glm::vec3& p1)
	{
		if (dirty == true) {
			RebuildOctree();
			dirty = false;
		}

		vector<int> pickedFaceIndices;

		vector<HeOctreeGeometry*> intersectingOctrees;
		octree->GetLineInersectingOctrees(p0, p1, intersectingOctrees);

		if (intersectingOctrees.size() > 0)
		{
			vector<tuple<float, int>> unorderedPickedFaceIndices;
			for (auto& ot : intersectingOctrees)
			{
				for (auto& fi : ot->GetContainingFaceIndices())
				{
					auto vi0 = GetIndex((int)fi * 3);
					auto vi1 = GetIndex((int)fi * 3 + 1);
					auto vi2 = GetIndex((int)fi * 3 + 2);

					auto& v0 = GetVertex(vi0);
					auto& v1 = GetVertex(vi1);
					auto& v2 = GetVertex(vi2);

					glm::vec3 intersection;
					if (glm::intersectLineTriangle(p0, p1, v0, v1, v2, intersection))
					{
						pickedFaceIndices.push_back((int)fi);
					}
				}
			}
		}

		return pickedFaceIndices;
	}

	void HeGeometry::PreDraw(HeCamera* camera)
	{

	}

	void HeGeometry::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
	{
		if (dirty == true) {
			if (dynamic_cast<HeGeometryThickLines*>(this) == nullptr)
			{
				//RebuildOctree();
				dirty = false;
			}
		}

		if (material == nullptr)
			return;

		material->Use(projection, view, model);

		vao->Bind();

		if(vbo->Size() > 0) vbo->Bind();
		if(vnbo->Size() > 0) vnbo->Bind();
		if(ibo->Size() > 0) ibo->Bind();
		if(cbo->Size() > 0) cbo->Bind();
		if(uvbo->Size() > 0) uvbo->Bind();

		CheckGLError();

		if (fillMode == Wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		CheckGLError();

		if (ibo->Size() > 0)
		{
			glDrawElements(drawingMode, (GLsizei)ibo->Size(), GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(drawingMode, 0, (GLsizei)vbo->Size());
		}

		CheckGLError();

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

	void HeGeometry::RebuildOctree()
	{
		if (octree != nullptr) {
			delete octree;
		}
		octree = new HeOctreeGeometry(this, aabb->GetMin(), aabb->GetMax(), 0);

		auto nof = GetIndexCount() / 3;
		for (size_t fi = 0; fi < nof; fi++)
		{
			auto vi0 = GetIndex((int)fi * 3);
			auto vi1 = GetIndex((int)fi * 3 + 1);
			auto vi2 = GetIndex((int)fi * 3 + 2);

			auto& v0 = GetVertex(vi0);
			auto& v1 = GetVertex(vi1);
			auto& v2 = GetVertex(vi2);

			octree->AddTriangle((int)fi, v0, v1, v2);
		}
	}
}
