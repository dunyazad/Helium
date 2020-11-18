#include "HeCamera.h"

namespace ArtificialNature {

	HeCamera::HeCamera(HeScene* scene)
		: HeSceneNode(scene)
	{
	}

	HeCamera::~HeCamera()
	{
	}

	HeOrthogonalCamera::HeOrthogonalCamera(HeScene* scene)
		: HeCamera(scene)
	{

	}

	HeOrthogonalCamera::~HeOrthogonalCamera()
	{

	}

	void HeOrthogonalCamera::Update(float dt)
	{
		viewMatrix = glm::identity<glm::mat4>();
		viewMatrix = glm::translate(viewMatrix, -position);

		if (aspectRatio > 1)
		{
			projectionMatrix = glm::ortho(aspectRatio * -width * 0.5f / zoomFactor, aspectRatio * width * 0.5f / zoomFactor, -height * 0.5f / zoomFactor, height * 0.5f / zoomFactor, -depth * 0.5f, depth * 0.5f);
		}
		else
		{
			projectionMatrix = glm::ortho(-width * 0.5f / zoomFactor, width * 0.5f / zoomFactor, 1 / aspectRatio * -height * 0.5f / zoomFactor, 1 / aspectRatio * height * 0.5f / zoomFactor, -depth * 0.5f, depth * 0.5f);
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

	void HeOrthogonalCamera::Render()
	{

	}
}