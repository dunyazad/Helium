#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeVertexArrayObject
	{
	public:
		HeVertexArrayObject();
		~HeVertexArrayObject();

		inline unsigned int ID() { return id; }

		void Bind();
		void Unbind();

	protected:
		unsigned int id;
	};

}
