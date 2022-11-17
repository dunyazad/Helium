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

		int textureSlot = 0;
		for (auto& kvp : textures)
		{
			auto texture = kvp.second;
			if (texture)
			{
				stringstream ss;
				ss << "texture" << textureSlot;
				shader->SetUniformInt(ss.str(), textureSlot);
				CheckGLError();

				texture->Bind(GL_TEXTURE0 + textureSlot);
				CheckGLError();

				if (texture->GetTarget() == GL_TEXTURE_CUBE_MAP)
				{
					//glDepthMask(GL_FALSE);
				}
			}

			textureSlot++;
		}
	}

	void HeMaterial::StopUse()
	{
		for (auto& kvp : textures)
		{
			auto texture = kvp.second;
			if (texture)
			{
				if (texture->GetTarget() == GL_TEXTURE_CUBE_MAP)
				{
					//glDepthMask(GL_TRUE);
				}

				texture->Unbind();
				CheckGLError();
			}
		}
	}
}