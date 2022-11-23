#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Material/HeMaterial.h>

namespace ArtificialNature {

	class HeMaterialMutiTexture : public HeMaterial
	{
	public:
		HeMaterialMutiTexture(const string& name);
		~HeMaterialMutiTexture();

		virtual void Use(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
		virtual void StopUse();

		inline HeTexture* GetTexture(const string& name) const
		{
			if (textures.count(name) == 0)
			{
				return nullptr;
			}
			else
			{
				auto texture = (*(textures.find(name))).second;
				return texture;
			}
		}
		inline void AddTexture(const string& name, HeTexture* texture) { textures[name] = texture; }

		const map<string, HeTexture*>& GetTextures() { return textures; }

		inline const vector<float>& GetTextureWeights() const { return textureWeights; }
		inline void SetTextureWeights(const vector<float>& weights) {
			if (weights.size() < 32)
			{
				for (size_t i = 0; i < weights.size(); i++)
				{
					textureWeights[i] = weights[i];
				}
			}
			else
			{
				for (size_t i = 0; i < 32; i++)
				{
					textureWeights[i] = weights[i];
				}
			}
		}
		inline void SetTextureWeight(int index, float weight) { if (index < 32) textureWeights[index] = weight; }

	protected:
		map<string, HeTexture*> textures;
		vector<float> textureWeights;
	};

}
