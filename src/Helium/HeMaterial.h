#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeShader;
	class HeTexture;
	class HeCamera;

	class HeMaterial
	{
	public:
		HeMaterial();
		~HeMaterial();

		void Use(HeCamera* camera);

		inline HeShader* GetShader() { return shader; }
		inline void SetShader(HeShader* shader) { this->shader = shader; }

		inline HeTexture* GetTexture() { return texture; }
		inline void SetTexture(HeTexture* Texture) { this->texture = texture; }

	protected:
		HeShader* shader = nullptr;
		HeTexture* texture = nullptr;
	};

}
