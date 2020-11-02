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

		inline size_t Size() { return indices.size(); }

		void AddIndex(unsigned int index)
		{
			indices.push_back(index);
		}

		void SetData(const vector<unsigned int>& indices)
		{
			copy(indices.begin(), indices.end(), this->indices.begin());
		}

		void Upload()
		{
			Bind();
			
			glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		}

	private:
		unsigned int id;
		vector<unsigned int> indices;
	};

}