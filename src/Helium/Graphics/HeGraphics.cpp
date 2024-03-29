#include <Helium/Graphics/HeGraphics.h>
#include <Helium/Graphics/Geometry/Geometry.h>
#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/Material/Material.h>
#include <Helium/Graphics/Texture/Texture.h>
#include <Helium/Graphics/Image/Image.h>
#include <Helium/Graphics/HeFrameBufferObject.h>

namespace ArtificialNature {

	HeGraphics::HeGraphics(const string& name, int windowWidth, int windowHeight)
		: HeObject(name), windowWidth(windowWidth), windowHeight(windowHeight)
	{
	}

	HeGraphics::~HeGraphics()
	{
		for (auto& kvp : geometries)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		geometries.clear();

		for (auto& kvp : materials)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		materials.clear();

		for (auto& kvp : textures)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		textures.clear();

		for (auto& kvp : images)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		images.clear();

		for (auto& kvp : frameBuffers)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		frameBuffers.clear();

		for (auto& kvp : shaders)
		{
			if (kvp.second != nullptr)
			{
				delete kvp.second;
				kvp.second = nullptr;
			}
		}
		shaders.clear();
	}

	HeGeometry* HeGraphics::GetGeometry(const string& name)
	{
		if (geometries.count(name) == 0)
		{
			geometries[name] = new HeGeometry(name);
		}

		return geometries[name];
	}

	HeGeometryPlane* HeGraphics::GetGeometryPlane(const string& name, float columnLength, float rowLength, unsigned int colomns, unsigned int rows, HePlaneType type)
	{
		if (geometries.count(name) == 0)
		{
			geometries[name] = new HeGeometryPlane(name, columnLength, rowLength, colomns, rows, type);
		}

		return dynamic_cast<HeGeometryPlane*>(geometries[name]);
	}

	HeGeometrySkybox* HeGraphics::GetSkyboxGeometry(const string& name)
	{
		if (geometries.count(name) == 0)
		{
			geometries[name] = new HeGeometrySkybox(name);
		}

		return dynamic_cast<HeGeometrySkybox*>(geometries[name]);
	}

	HeGeometryThickLines* HeGraphics::GetGeometryThickLines(const string& name)
	{
		if (geometries.count(name) == 0)
		{
			geometries[name] = new HeGeometryThickLines(name);
		}

		return dynamic_cast<HeGeometryThickLines*>(geometries[name]);
	}

	HeGeometryTriangleSoup* HeGraphics::GetGeometryTriangleSoup(const string& name)
	{
		if (geometries.count(name) == 0)
		{
			geometries[name] = new HeGeometryTriangleSoup(name);
		}

		return dynamic_cast<HeGeometryTriangleSoup*>(geometries[name]);
	}

	HeShader* HeGraphics::GetShader(const string& name)
	{
		if (shaders.count(name) != 0)
		{
			return shaders[name];
		}

		return nullptr;
	}

	HeShader* HeGraphics::GetShader(const string& name, const HeURL& vertexShaderFileURL, const HeURL& fragmentShaderFileURL)
	{
		if (shaders.count(name) == 0)
		{
			shaders[name] = new HeShader(name, vertexShaderFileURL, fragmentShaderFileURL);
		}

		return shaders[name];
	}

	HeShader* HeGraphics::GetShader(const string& name, const HeURL& vertexShaderFileURL, const HeURL& fragmentShaderFileURL, const HeURL& geometryShaderFileURL)
	{
		if (shaders.count(name) == 0)
		{
			shaders[name] = new HeShader(name, vertexShaderFileURL, fragmentShaderFileURL, geometryShaderFileURL);
		}

		return shaders[name];
	}

	HeMaterial* HeGraphics::GetMaterial(const string& name)
	{
		if (materials.count(name) == 0)
		{
			materials[name] = new HeMaterial(name);
		}

		return materials[name];
	}

	HeMaterialSingleTexture* HeGraphics::GetMaterialSingleTexture(const string& name)
	{
		if (materials.count(name) == 0)
		{
			materials[name] = new HeMaterialSingleTexture(name);
		}

		return dynamic_cast<HeMaterialSingleTexture*>(materials[name]);
	}

	HeMaterialMutiTexture* HeGraphics::GetMaterialMutiTexture(const string& name)
	{
		if (materials.count(name) == 0)
		{
			materials[name] = new HeMaterialMutiTexture(name);
		}

		return dynamic_cast<HeMaterialMutiTexture*>(materials[name]);
	}

	HeMaterialTextureArray* HeGraphics::GetMaterialTextureArray(const string& name)
	{
		if (materials.count(name) == 0)
		{
			materials[name] = new HeMaterialTextureArray(name);
		}

		return dynamic_cast<HeMaterialTextureArray*>(materials[name]);
	}

	HeMaterialReprojection* HeGraphics::GetMaterialReprojection(const string& name)
	{
		if (materials.count(name) == 0)
		{
			materials[name] = new HeMaterialReprojection(name);
		}

		return dynamic_cast<HeMaterialReprojection*>(materials[name]);
	}

	HeTexture* HeGraphics::GetTexture(const string& name, HeImage* image)
	{
		if (textures.count(name) == 0)
		{
			textures[name] = new HeTexture(name, image);
		}

		return textures[name];
	}

	HeTexture* HeGraphics::GetTexture(const string& name, int width, int height)
	{
		if (textures.count(name) == 0)
		{
			textures[name] = new HeTexture(name, width, height);
		}

		return textures[name];
	}

	HeTextureFloatData* HeGraphics::GetTextureFloatData(const string& name, float* data, int dataLength)
	{
		if (textures.count(name) == 0)
		{
			textures[name] = new HeTextureFloatData(name, data, dataLength);
		}

		return dynamic_cast<HeTextureFloatData*>(textures[name]);
	}

	HeTextureFloatData* HeGraphics::GetTextureFloatData(const string& name, const vector<float>& data)
	{
		if (textures.count(name) == 0)
		{
			textures[name] = new HeTextureFloatData(name, data);
		}

		return dynamic_cast<HeTextureFloatData*>(textures[name]);
	}

	HeTextureArray* HeGraphics::GetTextureArray(const string& name, const vector<HeImage*>& images)
	{
		if (textureArrays.count(name) == 0)
		{
			textureArrays[name] = new HeTextureArray(name, images);
		}

		return textureArrays[name];
	}

	HeCanvasTexture* HeGraphics::GetCanvasTexture(const string& name, HeImage* image)
	{
		if (textures.count(name) == 0)
		{
			textures[name] = new HeCanvasTexture(name, image);
		}

		return dynamic_cast<HeCanvasTexture*>(textures[name]);
	}

	HeCubemapTexture* HeGraphics::GetCubemapTexture(const string& name, const vector<HeImage*>& images)
	{
		if (textures.count(name) == 0)
		{
			textures[name] = new HeCubemapTexture(name, images);
		}

		return dynamic_cast<HeCubemapTexture*>(textures[name]);
	}

	HeImage* HeGraphics::GetImage(const string& name, const string& filePath, bool verticalFlip)
	{
		if (images.count(name) == 0)
		{
			images[name] = new HeImage(this, name, filePath, verticalFlip);
		}

		return images[name];
	}

	HeFontImage* HeGraphics::GetFontImage(const string& name, const string& fontFilePath)
	{
		if (images.count(name) == 0)
		{
			images[name] = new HeFontImage(this, name, fontFilePath);
		}

		return dynamic_cast<HeFontImage*>(images[name]);
	}

	HeCanvasImage* HeGraphics::GetCanvasImage(const string& name, int width, int height)
	{
		if (images.count(name) == 0)
		{
			images[name] = new HeCanvasImage(this, name, width, height);
		}

		return dynamic_cast<HeCanvasImage*>(images[name]);
	}

	HeFrameBufferObject* HeGraphics::GetFrameBuffer(const string& name)
	{
		if (frameBuffers.count(name) != 0)
		{
			return frameBuffers[name];
		}
		return nullptr;
	}

	HeFrameBufferObject* HeGraphics::GetFrameBuffer(const string& name, HeTexture* texture)
	{
		if (frameBuffers.count(name) == 0)
		{
			frameBuffers[name] = new HeFrameBufferObject(name, this, texture);
		}

		return frameBuffers[name];
	}

	HeFrameBufferObject* HeGraphics::GetFrameBuffer(const string& name, int width, int height)
	{
		if (frameBuffers.count(name) == 0)
		{
			frameBuffers[name] = new HeFrameBufferObject(name, this, width, height);
		}

		return frameBuffers[name];
	}

	void HeGraphics::RegisterRenderList(HeGeometry* geometry, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
	{
		auto material = geometry->GetMaterial();
		if (material != nullptr)
		{
			auto multiTextureMaterial = dynamic_cast<HeMaterialMutiTexture*>(material);
			if (multiTextureMaterial != nullptr) {
				for (auto& kvp : multiTextureMaterial->GetTextures())
				{
					auto texture = kvp.second;
					if (texture != nullptr) {
						if (texture->HasAlpha())
						{
							transparentRenderInfos.push_back(RenderInfo(geometry, geometry->GetMaterial(), projection, view, model));
						}
						else
						{
							opaqueRenderInfos[texture].push_back(RenderInfo(geometry, geometry->GetMaterial(), projection, view, model));
						}
					}
					else
					{
						opaqueRenderInfos[texture].push_back(RenderInfo(geometry, geometry->GetMaterial(), projection, view, model));
					}
				}
			}
			else
			{
				opaqueRenderInfos[nullptr].push_back(RenderInfo(geometry, geometry->GetMaterial(), projection, view, model));
			}
		}
		else
		{
			opaqueRenderInfos[nullptr].push_back(RenderInfo(geometry, geometry->GetMaterial(), projection, view, model));
		}
	}

	void HeGraphics::Flush()
	{
		for (auto& kvp : opaqueRenderInfos)
		{
			for (const auto& ri : kvp.second)
			{
				ri.geometry->Draw(ri.projection, ri.view, ri.model);
			}

			kvp.second.clear();
		}
		opaqueRenderInfos.clear();

		sort(transparentRenderInfos.begin(), transparentRenderInfos.end(), RenderInfoLess());

		for (auto& ri : transparentRenderInfos)
		{
			ri.geometry->Draw(ri.projection, ri.view, ri.model);
		}
		transparentRenderInfos.clear();

		if (serialCaptureFileNames.size() > 0) {
			auto image = this->GetCanvasImage("SerialCapture", this->windowWidth, this->windowHeight);
			image->CaptureFrame(serialCaptureFileNames[0]);
			serialCaptureFileNames.erase(serialCaptureFileNames.begin());

			if (onSerialCaptureCallback != nullptr) {
				onSerialCaptureCallback(serialCaptureIndex);
			}

			serialCaptureIndex++;

			if (serialCaptureFileNames.size() == 0) {
				serialCaptureIndex = 0;
				onSerialCaptureCallback = nullptr;
			}
		}
	}

}
