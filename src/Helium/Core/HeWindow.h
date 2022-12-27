#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeWindow
	{
	public:
		HeWindow();
		virtual ~HeWindow();

	protected:
		GLFWwindow* glfwWindow = nullptr;
	};

}