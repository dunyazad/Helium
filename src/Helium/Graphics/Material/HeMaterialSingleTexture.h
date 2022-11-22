#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Material/HeMaterial.h>

namespace ArtificialNature {

	class HeMaterialSingleTexture : public HeMaterial
	{
	public:
		HeMaterialSingleTexture(const string& name);
		~HeMaterialSingleTexture();

		virtual void Use(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		virtual void StopUse();

		inline HeTexture* GetTexture() const { return texture; }
		inline void SetTexture(HeTexture* texture) { this->texture = texture; }
		
	protected:
		HeTexture* texture = nullptr;
	};

}
