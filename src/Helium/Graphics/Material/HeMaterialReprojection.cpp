#include <Helium/Graphics/Material/HeMaterialReprojection.h>

#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/Texture/Texture.h>

#include <Helium/Scene/HeCamera.h>

namespace ArtificialNature {

	HeMaterialReprojection::HeMaterialReprojection(const string& name)
		: HeMaterial(name)
	{
	}

	HeMaterialReprojection::~HeMaterialReprojection()
	{
	}

	void HeMaterialReprojection::Use(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
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

	void HeMaterialReprojection::StopUse()
	{
		if (textureArray)
		{
			textureArray->Unbind();
			CheckGLError();
		}
	}
}
