#include "HeCameraManipulator.h"
#include "HeCamera.h"

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
            auto& p = camera->GetPosition();
            auto& t = camera->GetTargetPosition();

            float dist = glm::distance(t, p);

            camera->SetPosition(camera->GetPosition() + glm::vec3(0, 0, -1 * dist / 10));
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            auto& p = camera->GetPosition();
            auto& t = camera->GetTargetPosition();

            float dist = glm::distance(t, p);

            camera->SetPosition(camera->GetPosition() + glm::vec3(0, 0, 1 * dist / 10));
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
        {
            camera->SetPosition(camera->GetPosition() + glm::vec3(-0.1, 0, 0));
            camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(-0.1, 0, 0));
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
        {
            camera->SetPosition(camera->GetPosition() + glm::vec3(0.1, 0, 0));
            camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(0.1, 0, 0));
        }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            camera->SetPosition(camera->GetPosition() + glm::vec3(0, -0.1, 0));
            camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(0, -0.1, 0));
        }
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            camera->SetPosition(camera->GetPosition() + glm::vec3(0, 0.1, 0));
            camera->SetTargetPosition(camera->GetTargetPosition() + glm::vec3(0, 0.1, 0));
        }
	}

	void HeCameraManipulatorObital::OnMouse(GLFWwindow* window, double xpos, double ypos)
	{
	}

	void HeCameraManipulatorObital::OnWheel(GLFWwindow* window, double xoffset, double yoffset)
	{
	}

}
