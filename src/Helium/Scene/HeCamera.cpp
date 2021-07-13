#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeCamera::HeCamera(const string& name, HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight)
		: HeSceneNode(name, scene), viewportX(viewportX), viewportY(viewportY), viewportWidth(viewportWidth), viewportHeight(viewportHeight), aspectRatio(1), zoomFactor(1)
		, viewMatrix(glm::identity<glm::mat4>()), projectionMatrix(glm::identity<glm::mat4>()), targetPosition(glm::vec3()), upDirection(glm::vec3(0, 1, 0))
	{
	}

	HeCamera::~HeCamera()
	{
	}

	HeOrthogonalCamera::HeOrthogonalCamera(const string& name, HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight)
		: HeCamera(name, scene, viewportX, viewportY, viewportWidth, viewportHeight), width(1), height(1), depth(1)
	{
	}

	HeOrthogonalCamera::~HeOrthogonalCamera()
	{
	}

	void HeOrthogonalCamera::Update(float dt)
	{
		viewMatrix = glm::identity<glm::mat4>();
		viewMatrix = glm::translate(viewMatrix, -localPosition);

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

	HePerspectiveCamera::HePerspectiveCamera(const string& name, HeScene* scene, float viewportX, float viewportY, float viewportWidth, float viewportHeight)
		: HeCamera(name, scene, viewportX, viewportY, viewportWidth, viewportHeight)
	{
		localPosition = glm::vec3(0, 0, 1);
	}

	HePerspectiveCamera::~HePerspectiveCamera()
	{
	}

	void HePerspectiveCamera::Update(float dt)
	{
		viewMatrix = glm::lookAt(localPosition, targetPosition, upDirection);

		projectionMatrix = glm::perspective(45.0f, aspectRatio, 0.1f, 10000.0f);
	}

	void HePerspectiveCamera::Render()
	{

	}
}