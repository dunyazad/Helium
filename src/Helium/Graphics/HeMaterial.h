#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeShader;
	class HeTexture;

	class HeMaterial : public HeObject
	{
	public:
		HeMaterial(const string& name);
		~HeMaterial();

		void Use(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		void StopUse();

		inline HeShader* GetShader() { return shader; }
		inline void SetShader(HeShader* shader) { this->shader = shader; }

		inline HeTexture* GetTexture() { return texture; }
		inline void SetTexture(HeTexture* Texture) { this->texture = texture; }

	protected:
		HeShader* shader = nullptr;
		HeTexture* texture = nullptr;
	};

}
