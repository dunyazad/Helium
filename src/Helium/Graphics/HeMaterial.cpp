#include <Helium/Graphics/HeMaterial.h>

#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/Texture/Texture.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeMaterial::HeMaterial(const string& name)
		: HeObject(name)
	{
	}

	HeMaterial::~HeMaterial()
	{
	}

	void HeMaterial::Use(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model)
	{
		if (shader == nullptr)
			return;

		shader->Use();

		shader->SetUniformMat4("projection", projection);
		shader->SetUniformMat4("view", view);
		shader->SetUniformMat4("model", model);

		if (texture)
		{
			texture->Bind();
			CheckGLError();
		}
	}

	void HeMaterial::StopUse()
	{
		if (texture)
		{
			texture->Unbind();
			CheckGLError();
		}
	}
}