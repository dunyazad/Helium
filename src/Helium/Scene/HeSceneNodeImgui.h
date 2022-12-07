#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Scene/HeSceneNode.h>

namespace ArtificialNature {

	class HeSceneNodeImgui : public HeSceneNode
	{
	public:
		virtual void Update(float dt);
		virtual void Render(HeCamera* camera);

		inline const string& GetText() const { return text; }
		inline void SetText(const string& text) { this->text = text; }

	protected:
		HeSceneNodeImgui(const string& name, HeScene* scene);
		virtual ~HeSceneNodeImgui();

		ImFont* defaultFont = nullptr;
		ImFont* customFont = nullptr;

		string text;

	public:
		friend class HeScene;
	};

}