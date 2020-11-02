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

		template<typename T>
		void SetData(const vector<T>& vertices)
		{
			Bind();
			
			glBufferData(GL_ARRAY_BUFFER, vertices.size * sizeof(T), &vertices[0], GL_STATIC_DRAW);
		}

	private:
		unsigned int id;
	};

}