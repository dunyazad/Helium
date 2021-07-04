#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Graphics.h>
#include <Helium/Scene/Scene.h>

namespace ArtificialNature {

	class Helium
	{
	public:
		Helium();
		~Helium();

		inline const HeGraphics* GetGraphics() const { return graphics; }
		inline const list<HeScene*> GetScenes() const { return scenes; }

	private:
		HeGraphics* graphics = nullptr;
		list<HeScene*> scenes;
	};
}
