#pragma once

#include <Helium/Core/HeliumCommon.h>
#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	class HeGeometry;
	class HePlaneGeometry;
	class HeSkyboxGeometry;
	class HeThickLines;

	class HeShader;
	class HeMaterial;
	class HeTexture;
	class HeCanvasTexture;
	class HeCubemapTexture;
	class HeImage;
	class HeCanvasImage;
	class HeFrameBufferObject;

	class HeGraphics : public HeObject
	{
	public:
		HeGraphics(const string& name);
		~HeGraphics();

		HeGeometry* GetGeometry(const string& name);
		HePlaneGeometry* GetGeometryPlane(const string& name, float columnLength, float rowLength, unsigned int colomns, unsigned int rows, HePlaneType type);
		HeSkyboxGeometry* GetSkyboxGeometry(const string& name);
		HeThickLines* GetGeometryThickLines(const string& name);

		HeShader* GetShader(const string& name);
		HeShader* GetShader(const string& name, const string& vertexShaderFileName, string fragmentShaderFileName);
		HeShader* GetShader(const string& name, const string& vertexShaderFileName, string geometryShaderFileName, string fragmentShaderFileName);

		HeMaterial* GetMaterial(const string& name);

		HeTexture* GetTexture(const string& name, HeImage* image);
		HeTexture* GetTexture(const string& name, int width, int height);
		HeCanvasTexture* GetCanvasTexture(const string& name, HeImage* image);
		HeCubemapTexture* GetCubemapTexture(const string& name, const vector<HeImage*>& images);

		HeImage* GetImage(const string& name, const string& filePath, bool verticalFlip = true);
		HeCanvasImage* GetCanvasImage(const string& name, int width, int height);

		HeFrameBufferObject* GetFrameBuffer(const string& name);
		HeFrameBufferObject* GetFrameBuffer(const string& name, HeTexture* texture);
		HeFrameBufferObject* GetFrameBuffer(const string& name, int width, int height);

	private:
		map<string, HeGeometry*> geometries;
		map<string, HeShader*> shaders;
		map<string, HeMaterial*> materials;
		map<string, HeTexture*> textures;
		map<string, HeImage*> images;
		map<string, HeFrameBufferObject*> frameBuffers;
	};
}
