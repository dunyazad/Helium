#include <Helium/Core/HeResourceIO.h>

#include <Helium/Graphics/HeGraphics.h>
#include <Helium/Graphics/Geometry/Geometry.h>
#include <Helium/Core/HeFile.h>

using namespace tinyply;

namespace ArtificialNature {

	glm::quat GetRotation(const glm::vec3& v0, const glm::vec3& v1)
	{
		glm::quat q;
		glm::vec3 axis = glm::cross(v0, v1);
		q.x = axis.x;
		q.y = axis.y;
		q.z = axis.z;
		q.w = sqrt((glm::length(v0) * glm::length(v0)) * (glm::length(v1) * glm::length(v1))) + glm::dot(v0, v1);
		q = glm::normalize(q);
		return q;
	}

	HeGeometry* HeResourceIO::ReadPoints(HeGraphics* pGraphics, const string& name, const string& filepath)
	{
		HeFile file;
		file.Open(filepath, false);

		auto pGeometry = pGraphics->GetGeometryTriangleSoup(name);
		pGeometry->Initialize();

		float scale = 2.0f;

		string line;
		while (file.GetLine(line))
		{
			float px, py, pz, nx, ny, nz;
			int r, g, b, a;

			stringstream ss(line);
			ss >> px; ss >> py; ss >> pz;
			ss >> r; ss >> g; ss >> b; ss >> a;
			ss >> nx; ss >> ny; ss >> nz;

			glm::vec3 translation(px * 100, py * 100, pz * 100);
			glm::quat rotation = GetRotation(glm::vec3(0, 0, 1), glm::vec3(nx, ny, nz));

			glm::vec3 ll(-scale * 0.5f, -scale * 0.5f, 0.0f);
			glm::vec3 lr(-scale * 0.5f, +scale * 0.5f, 0.0f);
			glm::vec3 ul(+scale * 0.5f, -scale * 0.5f, 0.0f);
			glm::vec3 ur(+scale * 0.5f, +scale * 0.5f, 0.0f);

			pGeometry->AddTriangle(rotation * ll + translation, rotation * ul + translation, rotation * lr + translation, glm::vec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f));
			pGeometry->AddTriangle(rotation * lr + translation, rotation * ul + translation, rotation * ur + translation, glm::vec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, (float)a / 255.0f));
		}

		return pGeometry;
	}
}
