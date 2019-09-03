#include "stdafx.h"

#include <Helium/internal/_Scene.h>
#include <Helium/internal/_GraphicsDevice.h>

namespace ArtificialNature {

	_Scene::_Scene()
	{

	}

	_Scene::~_Scene()
	{

	}

	bool _Scene::IsActive()
	{
		return m_bActive;
	}

	void _Scene::SetActive(bool bActive)
	{
		m_bActive = bActive;
	}

	void _Scene::Render(GraphicsDevice* pGraphicsDevice)
	{
		glMatrixMode(GL_PROJECTION);
		glOrtho(-640, 640, -480, 480, -1, 1);

		glBegin(GL_TRIANGLES);

		glColor3f(0.5, 0, 0);

		glVertex3f(300.0, 0.0, 210.0);
		glVertex3f(340.0, 0.0, 215.0);
		glVertex3f(320.0, 0.0, 250.0);

		glEnd();

		//glFlush();
	}

}
