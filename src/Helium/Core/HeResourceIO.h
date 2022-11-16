#pragma once

#include <Helium/Core/HeliumCommon.h>

using namespace std::chrono;

namespace ArtificialNature {

	class HeGraphics;
	class HeGeometry;

	class HeResourceIO
	{
	public:
		static HeGeometry* ReadPoints(HeGraphics* pGraphics, const string& name, const string& filepath);
		static HeGeometry* ReadSTLFile(HeGraphics* pGraphics, const string& name, const string& filepath);
		static HeGeometry* ReadBinarySTLFile(HeGraphics* pGraphics, const string& name, const string& filepath);
		static HeGeometry* ReadASCIISTLFile(HeGraphics* pGraphics, const string& name, const string& filepath);
		static HeGeometry* ReadOBJFile(HeGraphics* pGraphics, const string& name, const string& filepath);

		static void WriteSTLFile(HeGraphics* pGraphics, const string& name, const string& filepath);
		static void WriteOBJFile(HeGraphics* pGraphics, const string& name, const string& filepath);

	protected:
		static void ParseOneLine(
			const string& line,
			vector<glm::vec3>& vertices,
			vector<glm::vec2>& uvs,
			vector<glm::vec3>& vertex_normals,
			vector<tuple<glm::ivec3, glm::ivec3, glm::ivec3>>& faces);
	};

}
