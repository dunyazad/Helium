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

	protected:
		HeShader* shader = nullptr;
		map<string, HeTexture*> textures;
	};

}
