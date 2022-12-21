#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Material/HeMaterial.h>

namespace ArtificialNature {

	class HeTextureFloatData;
	class HeTextureArray;

	class HeMaterialReprojection : public HeMaterial
	{
	public:
		HeMaterialReprojection(const string& name);
		~HeMaterialReprojection();

		virtual void Use(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
		virtual void StopUse();

		inline HeTextureFloatData* GetTextureFloatData() const { return textureFloatData; }
		inline void SetTextureFloatData(HeTextureFloatData* textureFloatData) { this->textureFloatData = textureFloatData; }

		inline HeTextureArray* GetTextureArray() const { return textureArray; }
		inline void SetTextureArray(HeTextureArray* textureArray) { this->textureArray = textureArray; }
		
		inline int GetIncremental() const { return incremental; }
		inline void SetIncremental(int incremental) { this->incremental = incremental; }

	protected:
		HeTextureFloatData* textureFloatData = nullptr;
		HeTextureArray* textureArray = nullptr;

		int incremental = 0;
	};

}
