#include <Helium/Graphics/Material/HeMaterialSingleTexture.h>

#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/Texture/Texture.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeMaterialSingleTexture::HeMaterialSingleTexture(const string& name)
		: HeMaterial(name)
	{
	}

	HeMaterialSingleTexture::~HeMaterialSingleTexture()
	{
	}

	void HeMaterialSingleTexture::Use(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
	{
		if (shader == nullptr)
			return;

		HeMaterial::Use(projection, view, model);

		if (texture)
		{
			texture->Bind();
			CheckGLError();

			if (texture->GetTarget() == GL_TEXTURE_CUBE_MAP)
			{
				//glDepthMask(GL_FALSE);
			}
		}
	}

	void HeMaterialSingleTexture::StopUse()
	{
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
