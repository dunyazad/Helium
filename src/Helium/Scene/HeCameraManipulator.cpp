#include <Helium/Scene/HeCameraManipulator.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeCameraManipulatorBase::HeCameraManipulatorBase(HeCamera* camera)
		: camera(camera)
	{
	}

	HeCameraManipulatorObital::HeCameraManipulatorObital(HeCamera* camera)
		: HeCameraManipulatorBase(camera)
	{
	}

	void HeCameraManipulatorObital::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			auto& p = camera->GetLocalPosition();
			auto& t = camera->GetTargetPosition();

			float dist = glm::distance(t, p);

			camera->SetLocalPosition(camera->GetLocalPosition() + glm::vec3(0, 0, -1 * dist / 10));
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			auto& p = camera->GetLocalPosition();
			auto& t = camera->GetTargetPosition();

			float dist = glm::distance(t, p);

			camera->SetLocalPosition(camera->GetLocalPosition() + glm::vec3(0, 0, 1 * dist / 10));
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			camera->SetLocalPosition(camera->GetLocalPosition() + glm::vec3(-0.1, 0, 0));
			camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(-0.1, 0, 0));
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			camera->SetLocalPosition(camera->GetLocalPosition() + glm::vec3(0.1, 0, 0));
			camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(0.1, 0, 0));
		}
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			camera->SetLocalPosition(camera->GetLocalPosition() + glm::vec3(0, -0.1, 0));
			camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(0, -0.1, 0));
		}
		if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			camera->SetLocalPosition(camera->GetLocalPosition() + glm::vec3(0, 0.1, 0));
			camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(0, 0.1, 0));
		}
	}

	void HeCameraManipulatorObital::OnMouse(GLFWwindow* window, double xpos, double ypos)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
		{
			auto offsetX = xpos - lastMousePositionX;
			auto offsetY = ypos - lastMousePositionY;
			lastMousePositionX = xpos;
			lastMousePositionY = ypos;
		}
	}

	void HeCameraManipulatorObital::OnWheel(GLFWwindow* window, double xoffset, double yoffset)
	{
		float zoomFactor = camera->GetZoomFactor();
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			zoomFactor -= (float)yoffset * zoomFactor * 0.5f;
		else
			zoomFactor -= (float)yoffset * zoomFactor * 0.05f;

		camera->SetZoomFactor(zoomFactor);
	}

}
