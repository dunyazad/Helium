#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Graphics.h>
#include <Helium/Scene/Scene.h>

namespace ArtificialNature {

	class Helium : public HeObject
	{
	public:
		Helium(const string& name);
		~Helium();

		HeScene* GetScene(const string& sceneName);

		inline HeGraphics* GetGraphics() const { return graphics; }
		inline const map<string, HeScene*> GetScenes() const { return scenes; }

	private:
		HeGraphics* graphics = nullptr;
		map<string, HeScene*> scenes;
	};
}
