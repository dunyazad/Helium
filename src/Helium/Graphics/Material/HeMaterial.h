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

		virtual void Use(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		virtual void StopUse();

		inline HeShader* GetShader() { return shader; }
		inline void SetShader(HeShader* shader) { this->shader = shader; }

	protected:
		HeShader* shader = nullptr;
	};

}
