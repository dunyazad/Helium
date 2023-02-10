#include <Helium/Scene/HeCameraManipulator.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeCameraManipulatorBase::HeCameraManipulatorBase(HeCamera* camera)
		: camera(camera)
	{
	}

	HeCameraManipulatorOrtho::HeCameraManipulatorOrtho(HeOrthogonalCamera* camera)
		: HeCameraManipulatorBase(camera)
	{

	}

	void HeCameraManipulatorOrtho::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void HeCameraManipulatorOrtho::OnMousePosition(GLFWwindow* window, double xpos, double ypos)
	{
		lastMousePositionX = xpos;
		lastMousePositionY = ypos;

		if (mouseRightButtonDown)
		{
			auto offsetX = xpos - lastMouseRightPositionX;
			auto offsetY = ypos - lastMouseRightPositionY;
			lastMouseRightPositionX = xpos;
			lastMouseRightPositionY = ypos;

			camera->SetLocalPosition(camera->GetLocalPosition() + glm::vec3(-offsetX * 0.001f, offsetY * 0.001f, 0));
		}
	}

	void HeCameraManipulatorOrtho::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
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

	void HeCameraManipulatorOrtho::OnWheel(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (yoffset < 0) {
			camera->SetZoomFactor(camera->GetZoomFactor() * 0.5f);
		}
		else {
			camera->SetZoomFactor(camera->GetZoomFactor() * 2.0f);
		}
	}

	void HeCameraManipulatorOrtho::Reset()
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

		if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			if (wireframeMode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				wireframeMode = false;
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				wireframeMode = true;
			}
		}

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			Reset();
		}

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto df = camera->GetCameraFront() * (float)(distance * 0.01);
			center = center + df;
			ApplyCamera();
		}
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto df = camera->GetCameraFront() * (float)(distance * 0.01);
			center = center - df;
			ApplyCamera();
		}
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto dr = camera->GetCameraRight() * (float)(distance * 0.01);
			center = center - dr;
			ApplyCamera();
		}
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto dr = camera->GetCameraRight() * (float)(distance * 0.01);
			center = center + dr;
			ApplyCamera();
		}
		if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto du = camera->GetCameraUp() * (float)(distance * 0.01);
			center = center + du;
			ApplyCamera();
		}
		if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto du = camera->GetCameraUp() * (float)(distance * 0.01);
			center = center - du;
			ApplyCamera();
		}
	}

	void HeCameraManipulatorObital::OnMousePosition(GLFWwindow* window, double xpos, double ypos)
	{
		lastMousePositionX = xpos;
		lastMousePositionY = ypos;

		if (mouseRightButtonDown)
		{
			auto offsetX = xpos - lastMouseRightPositionX;
			auto offsetY = ypos - lastMouseRightPositionY;
			lastMouseRightPositionX = xpos;
			lastMouseRightPositionY = ypos;

			angleH -= offsetX * 0.1f;
			angleV -= offsetY * 0.1f;

			if (angleV < -90.0f) angleV = -90.0f + 0.01f;
			if (angleV > 90.0f) angleV = 90.0f - 0.01f;
		}
		else if (mouseMiddleButtonDown)
		{
			auto offsetX = xpos - lastMouseMiddlePositionX;
			auto offsetY = ypos - lastMouseMiddlePositionY;
			lastMouseMiddlePositionX = xpos;
			lastMouseMiddlePositionY = ypos;

			auto dr = camera->GetCameraRight() * (float)(-offsetX * distance * 0.001);
			auto du = camera->GetCameraUp() * (float)(offsetY * distance * 0.001);
			center = center + (dr + du);
		}

		ApplyCamera();
	}

	void HeCameraManipulatorObital::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
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

	void HeCameraManipulatorObital::OnWheel(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			if (yoffset < 0)
			{
				distance *= 1.05f;
			}
			else
			{
				distance /= 1.05f;
			}
		}
		else
		{
			if (yoffset < 0)
			{
				distance *= 1.01f;
			}
			else
			{
				distance /= 1.01f;
			}
		}

		if (distance < 0.1f) distance = 0.1f;

		ApplyCamera();
	}

	void HeCameraManipulatorObital::Reset()
	{
		distance = 1.0f;
		angleH = 0.0f;
		angleV = 0.0f;
		center = glm::vec3(0, 0, 0);
	}
	
	void HeCameraManipulatorObital::ApplyCamera()
	{
		//auto eye = center +
		//	glm::vec3(
		//		distance * sin(glm::radians(angleH)) * cos(glm::radians(angleV)),
		//		distance * sin(glm::radians(angleH)) * sin(glm::radians(angleV)),
		//		distance * cos(glm::radians(angleH)));

		//cout << "distance: " << distance << " | eye: " << eye << " | center: " << center << endl;

		auto rH = glm::angleAxis(glm::radians(angleH), glm::vec3(0, 1, 0));
		auto rV = glm::angleAxis(glm::radians(angleV), glm::vec3(1, 0, 0));

		auto eye = center + (rH * rV) * glm::vec3(0, 0, distance);

		auto m = glm::lookAt(eye, center, glm::vec3(0, 1, 0));

		//camera->SetTargetPosition(center);
		//camera->SetLocalRotation(glm::inverse(m));
		//camera->SetLocalPosition(m[3]);
		camera->SetViewMatrix(m);
	}

	HeCameraManipulatorFlight::HeCameraManipulatorFlight(HeCamera* camera)
		: HeCameraManipulatorBase(camera)
	{
	}

	void HeCameraManipulatorFlight::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (key == GLFW_KEY_M && action == GLFW_PRESS)
		{
			if (wireframeMode) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				wireframeMode = false;
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				wireframeMode = true;
			}
		}
		
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			Reset();
		}

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto front = camera->GetCameraFront();
			camera->SetLocalPosition(camera->GetLocalPosition() + front);
		}
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto front = camera->GetCameraFront();
			camera->SetLocalPosition(camera->GetLocalPosition() - front);
		}
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto right = camera->GetCameraRight();
			camera->SetLocalPosition(camera->GetLocalPosition() - right);
		}
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto right = camera->GetCameraRight();
			camera->SetLocalPosition(camera->GetLocalPosition() + right);
		}
		if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto up = camera->GetCameraUp();
			camera->SetLocalPosition(camera->GetLocalPosition() + up);
		}
		if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto up = camera->GetCameraUp();
			camera->SetLocalPosition(camera->GetLocalPosition() - up);
		}
	}

	void HeCameraManipulatorFlight::OnMousePosition(GLFWwindow* window, double xpos, double ypos)
	{
		lastMousePositionX = xpos;
		lastMousePositionY = ypos;

		if (mouseRightButtonDown)
		{
			auto offsetX = xpos - lastMouseRightPositionX;
			auto offsetY = ypos - lastMouseRightPositionY;
			lastMouseRightPositionX = xpos;
			lastMouseRightPositionY = ypos;

			auto rotationH = glm::angleAxis(glm::radians((float)-offsetX * 0.1f), camera->GetCameraUp());
			auto rotationV = glm::angleAxis(glm::radians((float)-offsetY * 0.1f), camera->GetCameraRight());
			camera->SetLocalRotation(rotationH * rotationV * camera->GetLocalRotation());
		}
		else if (mouseMiddleButtonDown)
		{
			auto offsetX = xpos - lastMouseMiddlePositionX;
			auto offsetY = ypos - lastMouseMiddlePositionY;
			lastMouseMiddlePositionX = xpos;
			lastMouseMiddlePositionY = ypos;

			auto dr = camera->GetCameraRight() * (float)(-offsetX * 0.01);
			auto du = camera->GetCameraUp() * (float)(offsetY * 0.01);
			auto lp = camera->GetLocalPosition();
			camera->SetLocalPosition(dr + du + lp);
		}
	}

	void HeCameraManipulatorFlight::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
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

	void HeCameraManipulatorFlight::OnWheel(GLFWwindow* window, double xoffset, double yoffset)
	{
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		{
			if (yoffset < 0)
			{
				auto front = camera->GetCameraFront();
				camera->SetLocalPosition(camera->GetLocalPosition() - front);
			}
			else
			{
				auto front = camera->GetCameraFront();
				camera->SetLocalPosition(camera->GetLocalPosition() + front);
			}
		}
		else
		{
			if (yoffset < 0)
			{
				auto front = camera->GetCameraFront();
				camera->SetLocalPosition(camera->GetLocalPosition() - front * 0.1f);
			}
			else
			{
				auto front = camera->GetCameraFront();
				camera->SetLocalPosition(camera->GetLocalPosition() + front * 0.1f);
			}
		}
	}

	void HeCameraManipulatorFlight::Reset()
	{
		camera->SetLocalPosition(glm::vec3(0, 0, 0));
		camera->SetLocalRotation(glm::identity<glm::quat>());
	}

	HeCameraManipulatorTrackball::HeCameraManipulatorTrackball(HeCamera* camera)
		: HeCameraManipulatorBase(camera)
	{

	}

	void HeCameraManipulatorTrackball::OnKey(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
		}

		if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto front = camera->GetCameraFront() * (distance / 10.0f);

			camera->SetLocalPosition(camera->GetLocalPosition() + front);
			camera->SetTargetPosition(camera->GetTargetPosition() + front);
		}
		if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto front = camera->GetCameraFront() * (distance / 10.0f);

			camera->SetLocalPosition(camera->GetLocalPosition() - front);
			camera->SetTargetPosition(camera->GetTargetPosition() - front);
		}
		if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto right = camera->GetCameraRight() * (distance / 10.0f);

			camera->SetLocalPosition(camera->GetLocalPosition() - right);
			camera->SetTargetPosition(camera->GetTargetPosition() - right);
		}
		if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto right = camera->GetCameraRight() * (distance / 10.0f);

			camera->SetLocalPosition(camera->GetLocalPosition() + right);
			camera->SetTargetPosition(camera->GetTargetPosition() + right);
		}
		if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto up = camera->GetCameraUp() * (distance / 10.0f);

			camera->SetLocalPosition(camera->GetLocalPosition() + up);
			camera->SetTargetPosition(camera->GetTargetPosition() + up);
		}
		if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT))
		{
			auto up = camera->GetCameraUp() * (distance / 10.0f);

			camera->SetLocalPosition(camera->GetLocalPosition() - up);
			camera->SetTargetPosition(camera->GetTargetPosition() - up);
		}
	}

	void HeCameraManipulatorTrackball::OnMousePosition(GLFWwindow* window, double xpos, double ypos)
	{
		lastMousePositionX = xpos;
		lastMousePositionY = ypos;

		if (mouseRightButtonDown)
		{
			auto offsetX = xpos - lastMouseRightPositionX;
			auto offsetY = ypos - lastMouseRightPositionY;
			lastMouseRightPositionX = xpos;
			lastMouseRightPositionY = ypos;
		}
	}

	void HeCameraManipulatorTrackball::OnMouseButton(GLFWwindow* window, int button, int action, int mods)
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

	void HeCameraManipulatorTrackball::OnWheel(GLFWwindow* window, double xoffset, double yoffset)
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
	}

	void HeCameraManipulatorTrackball::Reset()
	{
	}
}
