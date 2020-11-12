#include "HeMaterial.h"
#include "HeShader.h"
#include "HeTexture.h"

namespace ArtificialNature {

	HeMaterial::HeMaterial()
	{
	}

	HeMaterial::~HeMaterial()
	{
	}

	void HeMaterial::Use()
	{
		if (shader == nullptr)
			return;

		shader->Use();

		glUniform1i(glGetUniformLocation(shader->GetProgram(), "texture1"), 0);

		CheckGLError();

		if (texture)
		{
			texture->Bind(GL_TEXTURE0, GL_TEXTURE_2D);
		}
	}
}