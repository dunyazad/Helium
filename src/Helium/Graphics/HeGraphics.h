#pragma once

#include <Helium/Core/HeliumCommon.h>
#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	class HeGeometry;
	class HePlane;
	class HeThickLines;

	class HeShader;
	class HeMaterial;
	class HeTexture;
	class HeCanvasTexture;
	class HeImage;

	class HeGraphics : public HeObject
	{
	public:
		HeGraphics(const string& name);
		~HeGraphics();

		HeGeometry* GetGeometry(const string& name);
		HePlane* GetGeometryPlane(const string& name, float columnLength, float rowLength, unsigned int colomns, unsigned int rows, HePlaneType type);
		HeThickLines* GetGeometryThickLines(const string& name);

		HeShader* GetShader(const string& name);
		HeShader* GetShader(const string& name, const string& vertexShaderFileName, string fragmentShaderFileName);
		HeShader* GetShader(const string& name, const string& vertexShaderFileName, string geometryShaderFileName, string fragmentShaderFileName);

		HeMaterial* GetMaterial(const string& name);

		HeTexture* GetTexture(const string& name, HeImage* image);
		HeCanvasTexture* GetCanvasTexture(const string& name, HeImage* image);

		HeImage* GetImage(const string& name, const string& filePath);

	private:
		map<string, HeGeometry*> geometries;
		map<string, HeShader*> shaders;
		map<string, HeMaterial*> materials;
		map<string, HeTexture*> textures;
		map<string, HeImage*> images;
	};
}
