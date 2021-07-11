#include <Helium/Helium.h>

namespace ArtificialNature {

	Helium::Helium(const string& name)
		: HeObject(name)
	{
		graphics = new HeGraphics(name + ".Graphics");
	}

	Helium::~Helium()
	{
		for (auto& kvp : scenes)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		scenes.clear();

		if (graphics != nullptr)
		{
			delete graphics;
			graphics = nullptr;
		}
	}

	HeScene* Helium::GetScene(const string& sceneName)
	{
		if (scenes.count(sceneName) == 0)
		{
			scenes[sceneName] = new HeScene(sceneName);
		}

		return scenes[sceneName];
	}
}
