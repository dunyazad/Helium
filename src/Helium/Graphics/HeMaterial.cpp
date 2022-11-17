#include <Helium/Graphics/HeMaterial.h>

#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/Texture/Texture.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeMaterial::HeMaterial(const string& name)
		: HeObject(name)
	{
		for (size_t i = 0; i < 32; i++)
		{
			textureWeights[i] = 0.0f;
		}
		textureWeights[0] = 1.0f;
	}

	HeMaterial::~HeMaterial()
	{
	}

	int temp = 0;
	void HeMaterial::Use(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model)
	{
		if (shader == nullptr)
			return;

		shader->Use();

		shader->SetUniformMat4("projection", projection);
		shader->SetUniformMat4("view", view);
		shader->SetUniformMat4("model", model);

		if (temp > 60)
		{
			if (textureWeights[0] > 0) {
				textureWeights[0] = 0.0f;
				textureWeights[1] = 1.0f;
			}
			else {
				textureWeights[0] = 1.0f;
				textureWeights[1] = 0.0f;
			}
			temp = 0;
		}
		temp++;


		shader->SetUniformFloatArray("textureWeights", textureWeights, 32);

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