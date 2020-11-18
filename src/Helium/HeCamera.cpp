#include "HeCamera.h"

namespace ArtificialNature {

	HeCamera::HeCamera(HeScene* scene)
		: HeSceneNode(scene)
	{

	}

	HeCamera::~HeCamera()
	{

	}

	void HeCamera::Update(float dt)
	{
		viewMatrix = glm::identity<glm::mat4>();

		if (projectionMode == Orthogonal)
		{

			projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
		}

		//if (projectionMode == Perspective)
		//{
		//	float x = sinf(glm::radians(90 - m_currentState.rotationV)) * sinf(glm::radians(180 - m_currentState.rotationH));
		//	float z = -sinf(glm::radians(90 - m_currentState.rotationV)) * cosf(glm::radians(180 - m_currentState.rotationH));
		//	float y = cosf(glm::radians(90 - m_currentState.rotationV));

		//	glm::vec3 direction = glm::normalize(glm::vec3(x, y, z));
		//	m_direction = -direction;
		//	m_position = m_currentState.targetPosition + direction * m_currentState.distance;

		//	m_viewMatrix = glm::lookAt(m_position, m_currentState.targetPosition, m_upVector);
		//}
		//else if (projectionMode == Orthogonal)
		//{

		//}
	}

	void HeCamera::Render()
	{

	}
}