#include <Helium/Graphics/Material/HeMaterialMutiTexture.h>

#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/Texture/Texture.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeMaterialMutiTexture::HeMaterialMutiTexture(const string& name)
		: HeMaterial(name)
	{
		textureWeights.resize(32);
		for (size_t i = 0; i < 32; i++)
		{
			textureWeights[i] = 0.0f;
		}
		textureWeights[0] = 1.0f;
	}

	HeMaterialMutiTexture::~HeMaterialMutiTexture()
	{
	}

	void HeMaterialMutiTexture::Use(const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model)
	{
		if (shader == nullptr)
			return;

		HeMaterial::Use(projection, view, model);

		shader->SetUniformFloatArray("textureWeights", textureWeights.data(), 32);

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

	void HeMaterialMutiTexture::StopUse()
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