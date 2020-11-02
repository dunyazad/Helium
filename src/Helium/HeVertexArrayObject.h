#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeVertexArrayObject
	{
	public:
		HeVertexArrayObject();
		~HeVertexArrayObject();

		inline unsigned int ID() { return id; }

		void Initialize();
		void Terminate();
		void Bind();
		void Unbind();

	protected:
		unsigned int id = -1;
	};

}
