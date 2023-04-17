#pragma once

#include <Helium/Core/Core.h>

namespace ArtificialNature {

	class HeCamera;
	class HeOrthogonalCamera;

	class HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorBase(HeCamera* camera);

		virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
		virtual void OnMousePosition(GLFWwindow* window, double xpos, double ypos) = 0;
		virtual void OnMouseButton(GLFWwindow* window, int button, int action, int mods) = 0;
		virtual void OnWheel(GLFWwindow* window, double xoffset, double yoffset) = 0;
		virtual void Reset() = 0;

		inline double GetLastMousePositionX() const { return lastMousePositionX; }
		inline double GetLastMousePositionY() const { return lastMousePositionY; }

	protected:
		HeCamera* camera;

		double lastMouseLeftPositionX = 0.0;
		double lastMouseLeftPositionY = 0.0;
		double lastMouseRightPositionX = 0.0;
		double lastMouseRightPositionY = 0.0;
		double lastMouseMiddlePositionX = 0.0;
		double lastMouseMiddlePositionY = 0.0;

		bool mouseLeftButtonDown = false;
		bool mouseRightButtonDown = false;
		bool mouseMiddleButtonDown = false;

		double lastMousePositionX = 0.0;
		double lastMousePositionY = 0.0;

		bool wireframeMode = false;
	};

	class HeCameraManipulatorOrtho : public HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorOrtho(HeOrthogonalCamera* camera);

		virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		virtual void OnMousePosition(GLFWwindow* window, double xpos, double ypos);
		virtual void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
		virtual void OnWheel(GLFWwindow* window, double xoffset, double yoffset);
		virtual void Reset();
	};

	class HeCameraManipulatorObital : public HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorObital(HeCamera* camera);

		virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		virtual void OnMousePosition(GLFWwindow* window, double xpos, double ypos);
		virtual void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
		virtual void OnWheel(GLFWwindow* window, double xoffset, double yoffset);
		virtual void Reset();

		void ApplyCamera();

		inline float GetDistance() const { return distance; }
		inline void SetDistance(float distance) { this->distance = distance; }

		inline float GetAngleH() const { return angleH; }
		inline void SetAngleH(float angleH) { this->angleH = angleH; }

		inline float GetAngleV() const { return angleV; }
		inline void SetAngleV(float angleV) { this->angleV = angleV; }

	protected:
		glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
		float angleH = 0.0f;
		float angleV = 0.0f;
		float distance = 1.0f;
	};

	class HeCameraManipulatorFlight : public HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorFlight(HeCamera* camera);

		virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		virtual void OnMousePosition(GLFWwindow* window, double xpos, double ypos);
		virtual void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
		virtual void OnWheel(GLFWwindow* window, double xoffset, double yoffset);
		virtual void Reset();
	};

	class HeCameraManipulatorTrackball : public HeCameraManipulatorBase
	{
	public:
		HeCameraManipulatorTrackball(HeCamera* camera);

		virtual void OnKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		virtual void OnMousePosition(GLFWwindow* window, double xpos, double ypos);
		virtual void OnMouseButton(GLFWwindow* window, int button, int action, int mods);
		virtual void OnWheel(GLFWwindow* window, double xoffset, double yoffset);
		virtual void Reset();

	protected:
		double lastMouseLeftPositionX = 0.0;
		double lastMouseLeftPositionY = 0.0;
		double lastMouseRightPositionX = 0.0;
		double lastMouseRightPositionY = 0.0;
		double lastMouseMiddlePositionX = 0.0;
		double lastMouseMiddlePositionY = 0.0;

		float distance = 10.0f;

		bool mouseLeftButtonDown = false;
		bool mouseRightButtonDown = false;
		bool mouseMiddleButtonDown = false;
	};
}