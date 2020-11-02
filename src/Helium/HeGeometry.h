#pragma once

#include "HeliumCommon.h"
#include "HeObject.h"

#include "HeVertexArrayObject.h"
#include "HeVertexBufferObject.h"
#include "HeIndexBufferObject.h"

namespace ArtificialNature {

	class HeGeometry : public HeObject
	{
	public:
		HeGeometry();
		~HeGeometry();

		void Initialize();
		void Terminate();

		void AddVertex(const glm::vec3& vertex);
		void AddIndex(unsigned int index);

		void Upload();

		void Draw();

	protected:
		HeVertexArrayObject vao;
		HeVertexBufferObject vbo;
		HeIndexBufferObject ibo;
	};

}
