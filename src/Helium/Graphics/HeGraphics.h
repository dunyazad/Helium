#pragma once

#include <Helium/Core/HeliumCommon.h>
#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	class HeGeometry;
	class HePlaneGeometry;
	class HeSkyboxGeometry;
	class HeThickLines;
	class HeTriangleSoupGeometry;

	class HeShader;
	class HeMaterial;
	class HeMaterialMutiTexture;
	class HeTexture;
	class HeCanvasTexture;
	class HeCubemapTexture;
	class HeImage;
	class HeFontImage;
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
		HeTriangleSoupGeometry* GetGeometryTriangleSoup(const string& name);

		HeShader* GetShader(const string& name);
		HeShader* GetShader(const string& name, const string& vertexShaderFileName, string fragmentShaderFileName);
		HeShader* GetShader(const string& name, const string& vertexShaderFileName, string geometryShaderFileName, string fragmentShaderFileName);

		HeMaterial* GetMaterial(const string& name);
		HeMaterialMutiTexture* GetMaterialMutiTexture(const string& name);

		HeTexture* GetTexture(const string& name, HeImage* image);
		HeTexture* GetTexture(const string& name, int width, int height);
		HeCanvasTexture* GetCanvasTexture(const string& name, HeImage* image);
		HeCubemapTexture* GetCubemapTexture(const string& name, const vector<HeImage*>& images);

		HeImage* GetImage(const string& name, const string& filePath, bool verticalFlip = true);
		HeFontImage* GetFontImage(const string& name, const string& fontFilePath);
		HeCanvasImage* GetCanvasImage(const string& name, int width, int height);

		HeFrameBufferObject* GetFrameBuffer(const string& name);
		HeFrameBufferObject* GetFrameBuffer(const string& name, HeTexture* texture);
		HeFrameBufferObject* GetFrameBuffer(const string& name, int width, int height);

		void RegisterRenderList(HeGeometry* geometry, const glm::mat4 projection, const glm::mat4 view, const glm::mat4 model);
		void Flush();

	private:
		map<string, HeGeometry*> geometries;
		map<string, HeShader*> shaders;
		map<string, HeMaterial*> materials;
		map<string, HeTexture*> textures;
		map<string, HeImage*> images;
		map<string, HeFrameBufferObject*> frameBuffers;

		struct RenderInfo {
			HeGeometry* geometry;
			HeMaterial* material;
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;

			RenderInfo(HeGeometry* geometry, HeMaterial* material, const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model)
				: geometry(geometry), material(material), projection(projection), view(view), model(model) {}
		};

		struct RenderInfoLess {
			inline bool operator() (const RenderInfo& a, const RenderInfo& b) {
				return glm::distance2(glm::vec3(a.view[3]), glm::vec3(a.model[3])) < glm::distance2(glm::vec3(b.view[3]), glm::vec3(b.model[3]));
			}
		};

		map<HeTexture*, vector<RenderInfo>> opaqueRenderInfos;
		vector<RenderInfo> transparentRenderInfos;
	};
}
