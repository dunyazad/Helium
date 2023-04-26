#pragma once

#include <Helium/Core/HeliumCommon.h>
#include <Helium/Core/HeFile.h>
#include <Helium/Core/HeURL.h>

using namespace std::chrono;

namespace ArtificialNature {

	class HeGraphics;
	class HeGeometry;
	class HeGeometryTriangleSoup;

	class HeResourceIO
	{
	public:
		static HeGeometry* ReadPoints(HeGraphics* pGraphics, const string& name, const string& filename, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
		static HeGeometry* ReadSTLFile(HeGraphics* pGraphics, const string& name, const HeURL& fileurl, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
		static HeGeometry* ReadBinarySTLFile(HeGraphics* pGraphics, const string& name, const string& filename, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
		static HeGeometry* ReadASCIISTLFile(HeGraphics* pGraphics, const string& name, const string& filename, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
		static HeGeometry* ReadOBJFile(HeGraphics* pGraphics, const string& name, const string& filename, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);

		static void WriteSTLFile(HeGraphics* pGraphics, const string& name, const string& filename, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
		static void WriteOBJFile(HeGraphics* pGraphics, const string& name, const string& filename, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);
		static void WriteOBJFile(HeGraphics* pGraphics, const vector<HeGeometryTriangleSoup*>& geometries, const string& filename, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f);

	protected:
		static void ParseOneLine(
			const string& line,
			vector<glm::vec3>& vertices,
			vector<glm::vec2>& uvs,
			vector<glm::vec3>& vertex_normals,
			vector<tuple<glm::ivec3, glm::ivec3, glm::ivec3>>& faces,
			float scaleX, float scaleY, float scaleZ);
	};

}
