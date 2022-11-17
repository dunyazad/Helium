#include <Helium/Scene/HeCameraManipulator.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeCameraManipulatorBase::HeCameraManipulatorBase(HeCamera* camera)
		: camera(camera)
	{
	}

	HeCameraManipulatorOrbital::HeCameraManipulatorOrbital(HeCamera* camera)
		: HeCameraManipulatorBase(camera)
	{
		ApplyManipulation();
	}

	void HeCameraManipulatorOrbital::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			rotationH = 0;
			rotationV = 0;

			ApplyManipulation();
		}

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto front = camera->GetCameraFront();

			camera->SetLocalPosition(camera->GetLocalPosition() + front);
			camera->SetTargetPosition(camera->GetTargetPosition() + front);
		}
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto front = camera->GetCameraFront();

			camera->SetLocalPosition(camera->GetLocalPosition() - front);
			camera->SetTargetPosition(camera->GetTargetPosition() - front);
		}
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto right = camera->GetCameraRight();

			camera->SetLocalPosition(camera->GetLocalPosition() - right);
			camera->SetTargetPosition(camera->GetTargetPosition() - right);
		}
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto right = camera->GetCameraRight();

			camera->SetLocalPosition(camera->GetLocalPosition() + right);
			camera->SetTargetPosition(camera->GetTargetPosition() + right);
		}
		if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto up = camera->GetCameraUp();

			camera->SetLocalPosition(camera->GetLocalPosition() + up);
			camera->SetTargetPosition(camera->GetTargetPosition() + up);
		}
		if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto up = camera->GetCameraUp();

			camera->SetLocalPosition(camera->GetLocalPosition() - up);
			camera->SetTargetPosition(camera->GetTargetPosition() - up);
		}
	}

	void HeCameraManipulatorOrbital::OnMousePosition(GLFWwindow* window, double xpos, double ypos)
	{
		if (mouseRightButtonDown)
		{
			auto offsetX = xpos - lastMouseRightPositionX;
			auto offsetY = ypos - lastMouseRightPositionY;
			lastMouseRightPositionX = xpos;
			lastMouseRightPositionY = ypos;

			rotationH -= (float)(offsetX * 0.005);
			rotationV += (float)(offsetY * 0.005);

			if (rotationV <= -glm::radians<float>(89))
			{
				rotationV = -glm::radians<float>(89);
			}

			if (rotationV >= glm::radians<float>(89))
			{
				rotationV = glm::radians<float>(89);
			}

			ApplyManipulation();
		}
	}

	void HeCameraManipulatorOrbital::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouseLeftButtonDown = true;

			lastMouseLeftPositionX = xpos;
			lastMouseLeftPositionY = ypos;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			mouseLeftButtonDown = false;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			mouseRightButtonDown = true;

			lastMouseRightPositionX = xpos;
			lastMouseRightPositionY = ypos;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			mouseRightButtonDown = false;
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		{
			mouseMiddleButtonDown = true;

			lastMouseMiddlePositionX = xpos;
			lastMouseMiddlePositionY = ypos;
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		{
			mouseMiddleButtonDown = false;
		}
	}

	void HeCameraManipulatorOrbital::OnWheel(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			distance -= (float)yoffset * 10;
		}
		else
		{
			distance -= (float)yoffset;
		}

		if (distance <= glm::epsilon<float>())
		{
			distance = glm::epsilon<float>();
		}

		ApplyManipulation();
	}

	void HeCameraManipulatorOrbital::ApplyManipulation()
	{
		auto rh = glm::angleAxis(rotationH, glm::vec3(0, 1, 0));
		auto rv = glm::angleAxis(-rotationV, rh * glm::vec3(1, 0, 0));

		auto& targetPosition = camera->GetTargetPosition();
		auto position = targetPosition + (rv * rh) * glm::vec3(0, 0, distance);
		camera->SetLocalPosition(position);
	}

}
