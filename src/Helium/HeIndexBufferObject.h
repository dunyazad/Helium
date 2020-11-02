#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeIndexBufferObject
	{
	public:
		HeIndexBufferObject();
		~HeIndexBufferObject();

		inline unsigned int ID() { return id; }

		void Initialize();
		void Terminate();

		void Bind();
		void Unbind();

		inline int Size() { return 0; }

		void SetData(const vector<unsigned int>& indices)
		{
			Bind();
			
			glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		}

	private:
		unsigned int id;
	};

}