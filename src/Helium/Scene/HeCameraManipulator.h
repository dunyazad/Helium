#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeCamera;

	class HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorBase(HeCamera* camera);

		virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
		virtual void OnMouse(GLFWwindow* window, double xpos, double ypos) = 0;
		virtual void OnWheel(GLFWwindow* window, double xoffset, double yoffset) = 0;

	protected:
		HeCamera* camera;
	};

	class HeCameraManipulatorObital : public HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorObital(HeCamera* camera);

		void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		void OnMouse(GLFWwindow* window, double xpos, double ypos);
		void OnWheel(GLFWwindow* window, double xoffset, double yoffset);

	};
}