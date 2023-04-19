#pragma once

#include <Helium/Core/Core.h>
#include <Helium/Graphics/Graphics.h>
#include <Helium/Scene/Scene.h>

namespace ArtificialNature {

	class Helium : public HeObject
	{
	public:
		Helium(const string& name, int windowWidth, int windowHeight);
		~Helium();

		HeScene* GetScene(const string& sceneName);

		inline HeGraphics* GetGraphics() const { return graphics; }
		inline const map<string, HeScene*> GetScenes() const { return scenes; }

		void InitializeImgui(GLFWwindow* window);
		void TerminateImgui();

		void Run();

		inline void OnPrepare(function<void()> callback) { callbackOnPrepare = callback; }
		inline void OnFrame(function<void()> callback) { callbackOnFrame = callback; }
		inline void OnTerminate(function<void()> callback) { callbackOnTerminate = callback; }

		inline void SetFinished(bool finished) { this->finished = finished; }

	private:
		int windowWidth = 1024;
		int windowHeight = 768;

		HeGraphics* graphics = nullptr;
		map<string, HeScene*> scenes;

		function<void()> callbackOnPrepare;
		function<void()> callbackOnFrame;
		function<void()> callbackOnTerminate;

		bool finished = false;
	};
}
