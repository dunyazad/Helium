#include <Helium/Graphics/HeGraphics.h>
#include <Helium/Graphics/Geometry/Geometry.h>
#include <Helium/Graphics/HeShader.h>
#include <Helium/Graphics/HeMaterial.h>
#include <Helium/Graphics/Texture/Texture.h>
#include <Helium/Graphics/Image/Image.h>

namespace ArtificialNature {

	HeGraphics::HeGraphics(const string& name)
		: HeObject(name)
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

	HePlane* HeGraphics::GetGeometryPlane(const string& name, float columnLength, float rowLength, unsigned int colomns, unsigned int rows, HePlaneType type)
	{
		if (geometries.count(name) == 0)
		{
			geometries[name] = new HePlane(name, columnLength, rowLength, colomns, rows, type);
		}

		return dynamic_cast<HePlane*>(geometries[name]);
	}

	HeThickLines* HeGraphics::GetGeometryThickLines(const string& name)
	{
		if (geometries.count(name) == 0)
		{
			geometries[name] = new HeThickLines(name);
		}

		return dynamic_cast<HeThickLines*>(geometries[name]);
	}

	HeShader* HeGraphics::GetShader(const string& name)
	{
		if (shaders.count(name) != 0)
		{
			return shaders[name];
		}

		return nullptr;
	}

	HeShader* HeGraphics::GetShader(const string& name, const string& vertexShaderFileName, string fragmentShaderFileName)
	{
		if (shaders.count(name) == 0)
		{
			shaders[name] = new HeShader(name, vertexShaderFileName, fragmentShaderFileName);
		}

		return shaders[name];
	}

	HeShader* HeGraphics::GetShader(const string& name, const string& vertexShaderFileName, string geometryShaderFileName, string fragmentShaderFileName)
	{
		if (shaders.count(name) == 0)
		{
			shaders[name] = new HeShader(name, vertexShaderFileName, geometryShaderFileName, fragmentShaderFileName);
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

	HeCanvasTexture* HeGraphics::GetCanvasTexture(const string& name, HeImage* image)
	{
		if (textures.count(name) == 0)
		{
			textures[name] = new HeCanvasTexture(name, image);
		}

		return dynamic_cast<HeCanvasTexture*>(textures[name]);
	}

	HeImage* HeGraphics::GetImage(const string& name, const string& filePath)
	{
		if (images.count(name) == 0)
		{
			images[name] = new HeImage(name, filePath);
		}

		return images[name];
	}

	HeCanvasImage* HeGraphics::GetCanvasImage(const string& name, int width, int height)
	{
		if (images.count(name) == 0)
		{
			images[name] = new HeCanvasImage(name, width, height);
		}

		return dynamic_cast<HeCanvasImage*>(images[name]);
	}
}