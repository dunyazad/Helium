#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeCamera;

	class HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorBase(HeCamera* camera);

		virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
		virtual void OnMousePosition(GLFWwindow* window, double xpos, double ypos) = 0;
		virtual void OnMouseButton(GLFWwindow* window, int button, int action, int mods) = 0;
		virtual void OnWheel(GLFWwindow* window, double xoffset, double yoffset) = 0;

	protected:
		HeCamera* camera;
	};

	class HeCameraManipulatorOrbital : public HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorOrbital(HeCamera* camera);

		void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		void OnMousePosition(GLFWwindow* window, double xpos, double ypos);
		void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
		void OnWheel(GLFWwindow* window, double xoffset, double yoffset);

		void ApplyManipulation();

	protected:
		double lastMouseLeftPositionX = 0.0;
		double lastMouseLeftPositionY = 0.0;
		double lastMouseRightPositionX = 0.0;
		double lastMouseRightPositionY = 0.0;
		double lastMouseMiddlePositionX = 0.0;
		double lastMouseMiddlePositionY = 0.0;

		float distance = 10.0f;
		float rotationH = glm::radians<float>(45.0f);
		float rotationV = glm::radians<float>(45.0f);

		bool mouseLeftButtonDown = false;
		bool mouseRightButtonDown = false;
		bool mouseMiddleButtonDown = false;
	};
}