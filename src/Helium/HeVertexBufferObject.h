#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeVertexBufferObject
	{
	public:
		HeVertexBufferObject();
		~HeVertexBufferObject();

		inline unsigned int ID() { return id; }

		void Initialize();
		void Terminate();

		void Bind();
		void Unbind();

		void AddVertex(const glm::vec3& vertex)
		{
			vertices.push_back(vertex);
		}

		//void SetData(const vector<glm::vec3>& vertices)
		//{
		//	copy(vertices.begin(), vertices.end(), this->vertices.begin());
		//}

		void Upload()
		{
			Bind();

			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size() * 3, &vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
			glEnableVertexAttribArray(0);
		}

	private:
		unsigned int id;
		vector<glm::vec3> vertices;
	};

}