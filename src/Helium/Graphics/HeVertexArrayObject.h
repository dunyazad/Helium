#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeVertexArrayObject
	{
	public:
		HeVertexArrayObject();
		~HeVertexArrayObject();

		inline unsigned int ID() { return id; }

		virtual void Initialize();
		virtual void Terminate();
		void Bind();
		void Unbind();

	protected:
		unsigned int id = -1;
	};

}
