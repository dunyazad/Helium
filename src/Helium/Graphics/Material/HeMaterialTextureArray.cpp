#include <Helium/Graphics/Material/HeMaterialTextureArray.h>

#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/Texture/Texture.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeMaterialTextureArray::HeMaterialTextureArray(const string& name)
		: HeMaterial(name)
	{
	}

	HeMaterialTextureArray::~HeMaterialTextureArray()
	{
	}

	void HeMaterialTextureArray::Use(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
	{
		if (shader == nullptr)
			return;

		HeMaterial::Use(projection, view, model);

		if (textureArray)
		{
			textureArray->Bind();

			shader->SetUniformInt("textureIndex", textureIndex);

			CheckGLError();
		}
	}

	void HeMaterialTextureArray::StopUse()
	{
		if (textureArray)
		{
			textureArray->Unbind();
			CheckGLError();
		}
	}
}
