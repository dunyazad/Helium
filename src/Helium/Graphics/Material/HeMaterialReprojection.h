#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Material/HeMaterial.h>

namespace ArtificialNature {

	class HeTextureArray;

	class HeMaterialReprojection : public HeMaterial
	{
	public:
		HeMaterialReprojection(const string& name);
		~HeMaterialReprojection();

		virtual void Use(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
		virtual void StopUse();

		inline HeTextureArray* GetTextureArray() const { return textureArray; }
		inline void SetTextureArray(HeTextureArray* textureArray) { this->textureArray = textureArray; }
		
		inline int GetTextureIndex() const { return textureIndex; }
		inline void SetTextureIndex(int textureIndex) { this->textureIndex = textureIndex; }

	protected:
		HeTextureArray* textureArray = nullptr;

		int textureIndex = 0;


	};

}
