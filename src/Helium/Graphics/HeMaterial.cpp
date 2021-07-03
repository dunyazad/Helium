#include <Helium/Graphics/HeMaterial.h>

#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/HeTexture.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeMaterial::HeMaterial()
	{
	}

	HeMaterial::~HeMaterial()
	{
	}

	void HeMaterial::Use(HeCamera* camera)
	{
		if (shader == nullptr)
			return;

		shader->Use();

		if (camera != nullptr)
		{
			shader->SetUniformMat4("view", camera->GetViewMatrix());
			shader->SetUniformMat4("projection", camera->GetProjectionMatrix());
		}

		if (texture)
		{
			texture->Bind(GL_TEXTURE0, GL_TEXTURE_2D);
			CheckGLError();

			glUniform1i(glGetUniformLocation(shader->GetProgram(), "texture1"), 0);
			CheckGLError();
		}
	}
}