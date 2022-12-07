#include <Helium/Core/HeResourceIO.h>

#include <Helium/Graphics/Graphics.h>
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

	HeGeometry* HeResourceIO::ReadPoints(HeGraphics* pGraphics, const string& name, const string& filename)
	{
		HeFile file;
		file.Open(filename, false);

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

	HeGeometry* HeResourceIO::ReadSTLFile(HeGraphics* pGraphics, const string& name, const string& filename)
	{
		ifstream ifs(filename);
		if (ifs.is_open() == false)
			return nullptr;

		string solid = "      ";
		ifs.read(&solid[0], 6);

		if (solid == "solid ")
		{
			ifs.close();
			return ReadASCIISTLFile(pGraphics, name, filename);
		}
		else
		{
			ifs.close();
			return ReadBinarySTLFile(pGraphics, name, filename);
		}

		return nullptr;
	}

	HeGeometry* HeResourceIO::ReadBinarySTLFile(HeGraphics* pGraphics, const string& name, const string& filename)
	{
		auto pGeometry = pGraphics->GetGeometryTriangleSoup(name);
		pGeometry->Initialize();

		FILE* fp = nullptr;
		fopen_s(&fp, filename.c_str(), "rb");
		if (fp != nullptr)
		{
			char header[80];
			memset(header, 0, 80);
			fread_s(header, 80, 80, 1, fp);

			int nof = 0;
			fread_s(&nof, 4, 4, 1, fp);

			for (size_t i = 0; i < nof; i++)
			{
				glm::vec3 fn, v0, v1, v2;
				short dummy;

				fread_s(&fn, 12, 12, 1, fp);
				fread_s(&v0, 12, 12, 1, fp);
				fread_s(&v1, 12, 12, 1, fp);
				fread_s(&v2, 12, 12, 1, fp);
				fread_s(&dummy, 2, 2, 1, fp);

				pGeometry->AddTriangle(v0, v1, v2, fn, fn, fn);
			}
		}

		return pGeometry;
	}

	HeGeometry* HeResourceIO::ReadASCIISTLFile(HeGraphics* pGraphics, const string& name, const string& filename)
	{
		auto pGeometry = pGraphics->GetGeometryTriangleSoup(name);
		pGeometry->Initialize();

		ifstream ifs(filename);
		stringstream buffer;
		buffer << ifs.rdbuf();

		glm::vec3 fn;
		glm::vec3 vertices[3];
		int vertex_index = 0;

		string line;
		while (buffer.good())
		{
			getline(buffer, line);
			if (line.empty())
				continue;

			auto words = split(line, " \t");
			if (words[0] == "facet")
			{
				if (words[1] == "normal")
				{
					fn.x = safe_stof(words[2]);
					fn.y = safe_stof(words[3]);
					fn.z = safe_stof(words[4]);
				}
			}
			else if (words[0] == "vertex")
			{
				vertices[vertex_index].x = safe_stof(words[1]);
				vertices[vertex_index].y = safe_stof(words[2]);
				vertices[vertex_index].z = safe_stof(words[3]);
				vertex_index++;
			}
			else if (words[0] == "endfacet")
			{
				pGeometry->AddTriangle(vertices[0], vertices[1], vertices[2], fn, fn, fn);
				vertex_index = 0;
			}
		}

		return pGeometry;
	}

	HeGeometry* HeResourceIO::ReadOBJFile(HeGraphics* pGraphics, const string& name, const string& filename)
	{
		auto pGeometry = pGraphics->GetGeometryTriangleSoup(name);
		pGeometry->Initialize();

		vector<glm::vec3> vertices;
		vector<glm::vec2> uvs;
		vector<glm::vec3> vertex_normals;
		vector<tuple<glm::ivec3, glm::ivec3, glm::ivec3>> faces;

		ifstream ifs(filename);
		stringstream buffer;
		buffer << ifs.rdbuf();

		string line;
		while (buffer.good())
		{
			getline(buffer, line);
			ParseOneLine(line, vertices, uvs, vertex_normals, faces);
		}

		for (auto& face : faces)
		{
			auto& fi0 = get<0>(face);
			auto& fi1 = get<1>(face);
			auto& fi2 = get<2>(face);

			bool ti_ok = fi0.y != INT_MAX && fi1.y != INT_MAX && fi2.y != INT_MAX;
			bool ni_ok = fi0.z != INT_MAX && fi1.z != INT_MAX && fi2.z != INT_MAX;

			if (ti_ok && ni_ok)
			{
				auto& v0 = vertices[fi0[0] - 1];
				auto& v1 = vertices[fi1[0] - 1];
				auto& v2 = vertices[fi2[0] - 1];

				auto& uv0 = uvs[fi0[1] - 1];
				auto& uv1 = uvs[fi1[1] - 1];
				auto& uv2 = uvs[fi2[1] - 1];

				auto& vn0 = vertex_normals[fi0[2] - 1];
				auto& vn1 = vertex_normals[fi1[2] - 1];
				auto& vn2 = vertex_normals[fi2[2] - 1];

				pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2, vn0, vn1, vn2);
			}
			else if (ti_ok)
			{
				auto& v0 = vertices[fi0[0] - 1];
				auto& v1 = vertices[fi1[0] - 1];
				auto& v2 = vertices[fi2[0] - 1];

				auto& uv0 = uvs[fi0[1] - 1];
				auto& uv1 = uvs[fi1[1] - 1];
				auto& uv2 = uvs[fi2[1] - 1];

				pGeometry->AddTriangle(v0, v1, v2, uv0, uv1, uv2);
			}
			else if (ni_ok)
			{
				auto& v0 = vertices[fi0[0] - 1];
				auto& v1 = vertices[fi1[0] - 1];
				auto& v2 = vertices[fi2[0] - 1];

				auto& vn0 = vertex_normals[fi0[2] - 1];
				auto& vn1 = vertex_normals[fi1[2] - 1];
				auto& vn2 = vertex_normals[fi2[2] - 1];

				pGeometry->AddTriangle(v0, v1, v2, vn0, vn1, vn2);
			}
			else
			{
			auto& v0 = vertices[fi0[0] - 1];
			auto& v1 = vertices[fi1[0] - 1];
			auto& v2 = vertices[fi2[0] - 1];

			pGeometry->AddTriangle(v0, v1, v2);
			}
		}

		return pGeometry;
	}

	void HeResourceIO::ParseOneLine(
		const string& line,
		vector<glm::vec3>& vertices,
		vector<glm::vec2>& uvs,
		vector<glm::vec3>& vertex_normals,
		vector<tuple<glm::ivec3, glm::ivec3, glm::ivec3>>& faces)
	{
		if (line.empty())
			return;

		auto words = split(line, " \t");

		if (words[0] == "v")
		{
			float x = safe_stof(words[1]);
			float y = safe_stof(words[2]);
			float z = safe_stof(words[3]);
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if (words[0] == "vt")
		{
			float u = safe_stof(words[1]);
			float v = safe_stof(words[2]);
			uvs.push_back(glm::vec2(u, v));
		}
		else if (words[0] == "vn")
		{
			float x = safe_stof(words[1]);
			float y = safe_stof(words[2]);
			float z = safe_stof(words[3]);
			vertex_normals.push_back(glm::vec3(x, y, z));
		}
		else if (words[0] == "f")
		{
			if (words.size() == 4)
			{
				auto fe0 = split(words[1], "/", true);
				auto fe1 = split(words[2], "/", true);
				auto fe2 = split(words[3], "/", true);

				if (fe0.size() == 1 && fe1.size() == 1 && fe2.size() == 1) {
					faces.push_back(
						make_tuple(
							glm::ivec3(safe_stoi(fe0[0]), INT_MAX, INT_MAX),
							glm::ivec3(safe_stoi(fe1[0]), INT_MAX, INT_MAX),
							glm::ivec3(safe_stoi(fe2[0]), INT_MAX, INT_MAX)));
				}
				else {
					faces.push_back(
						make_tuple(
							glm::ivec3(safe_stoi(fe0[0]), safe_stoi(fe0[1]), safe_stoi(fe0[2])),
							glm::ivec3(safe_stoi(fe1[0]), safe_stoi(fe1[1]), safe_stoi(fe1[2])),
							glm::ivec3(safe_stoi(fe2[0]), safe_stoi(fe2[1]), safe_stoi(fe2[2]))));
				}
			}
			else if (words.size() == 5)
			{
				auto fe0 = split(words[1], "/", true);
				auto fe1 = split(words[2], "/", true);
				auto fe2 = split(words[3], "/", true);
				auto fe3 = split(words[3], "/", true);

				faces.push_back(
					make_tuple(
						glm::ivec3(safe_stoi(fe0[0]), safe_stoi(fe0[1]), safe_stoi(fe0[2])),
						glm::ivec3(safe_stoi(fe1[0]), safe_stoi(fe1[1]), safe_stoi(fe1[2])),
						glm::ivec3(safe_stoi(fe2[0]), safe_stoi(fe2[1]), safe_stoi(fe2[2]))));

				faces.push_back(
					make_tuple(
						glm::ivec3(safe_stoi(fe0[0]), safe_stoi(fe0[1]), safe_stoi(fe0[2])),
						glm::ivec3(safe_stoi(fe2[0]), safe_stoi(fe2[1]), safe_stoi(fe2[2])),
						glm::ivec3(safe_stoi(fe3[0]), safe_stoi(fe3[1]), safe_stoi(fe3[2]))));
			}
		}
	}

	void HeResourceIO::WriteSTLFile(HeGraphics* pGraphics, const string& name, const string& filename)
	{
		auto pGeometry = pGraphics->GetGeometryTriangleSoup(name);
		if (pGeometry == nullptr) {
			return;
		}

		FILE* fp = nullptr;
		fopen_s(&fp, filename.c_str(), "wb");

		if (fp != nullptr)
		{
			if (pGeometry->GetNormalCount() == 0)
			{
				pGeometry->ComputeFaceNormals();
			}

			char header[80];
			memset(header, 0, 80);
			char header_string[] = "MeshIO";
			memcpy(header, header_string, 6);
			fwrite(header, 80, 1, fp);

			int nof = (int)pGeometry->GetIndexCount() / 3;
			fwrite(&nof, 4, 1, fp);

			int buffer_index = 0;
			char* buffer = new char[nof * 50];
			memset(buffer, 0, nof * 50);

			for (int i = 0; i < nof; i++)
			{
				auto vi0 = pGeometry->GetIndex(i * 3);
				auto vi1 = pGeometry->GetIndex(i * 3 + 1);
				auto vi2 = pGeometry->GetIndex(i * 3 + 2);

				auto& v0 = pGeometry->GetVertex(vi0);
				auto& v1 = pGeometry->GetVertex(vi1);
				auto& v2 = pGeometry->GetVertex(vi2);

				auto& fn = pGeometry->GetNormal(vi0);
				short dummy = 0;

				memcpy(buffer + buffer_index, &fn, 12); buffer_index += 12;
				memcpy(buffer + buffer_index, &v0, 12); buffer_index += 12;
				memcpy(buffer + buffer_index, &v1, 12); buffer_index += 12;
				memcpy(buffer + buffer_index, &v2, 12); buffer_index += 12;
				memcpy(buffer + buffer_index, &dummy, 2); buffer_index += 2;
			}

			fwrite(buffer, nof * 50, 1, fp);

			delete buffer;
		}

		fclose(fp);
	}

	void HeResourceIO::WriteOBJFile(HeGraphics* pGraphics, const string& name, const string& filename)
	{
		auto pGeometry = pGraphics->GetGeometryTriangleSoup(name);
		if (pGeometry == nullptr) {
			return;
		}

		ofstream ofs(filename);
		stringstream ss;
		ss.precision(4);

		ss << "# MeshIO" << endl;

		auto nov = (int)pGeometry->GetVertexCount();
		for (int i = 0; i < nov; i++)
		{
			auto& v = pGeometry->GetVertex(i);
			ss << "v " << v << endl;
		}

		auto nouv = (int)pGeometry->GetUVCount();
		for (int i = 0; i < nouv; i++)
		{
			auto& uv = pGeometry->GetUV(i);
			ss << "vt " << uv << endl;
		}

		auto non = (int)pGeometry->GetNormalCount();
		for (int i = 0; i < non; i++)
		{
			auto& vn = pGeometry->GetNormal(i);
			ss << "vn " << vn << endl;
		}

		bool has_uv = pGeometry->GetUVCount() != 0;
		bool has_vn = pGeometry->GetNormalCount() != 0;

		auto nof = (int)pGeometry->GetIndexCount() / 3;
		for (int i = 0; i < nof; i++)
		{
			GLuint face[3] = { (GLuint)i * 3 + 1, (GLuint)i * 3 + 2, (GLuint)i * 3 + 3 };
		
			if (has_uv && has_vn)
			{
				ss << "f "
					<< face[0] << "/" << face[0] << "/" << face[0] << " "
					<< face[1] << "/" << face[1] << "/" << face[1] << " "
					<< face[2] << "/" << face[2] << "/" << face[2] << endl;
			}
			else if (has_uv)
			{
				ss << "f "
					<< face[0] << "/" << face[0] << " "
					<< face[1] << "/" << face[1] << " "
					<< face[2] << "/" << face[2] << endl;
			}
			else if (has_vn)
			{
				ss << "f "
					<< face[0] << "//" << face[0] << " "
					<< face[1] << "//" << face[1] << " "
					<< face[2] << "//" << face[2] << endl;
			}
			else
			{
				ss << "f " << face[0] << " " << face[1] << " " << face[2] << endl;
			}
		}

		ofs << ss.rdbuf();
		ofs.close();
	}

	void HeResourceIO::WriteOBJFile(HeGraphics* pGraphics, const vector<HeGeometryTriangleSoup*>& geometries, const string& filename)
	{
		auto filepath = filesystem::path(filename);
		auto directory = filepath.parent_path();
		auto stem = filepath.stem();

		ofstream ofs(filename);
		stringstream ss;
		ss.precision(4);

		ss << "# MeshIO" << endl;
		ss << "mtllib " << stem.string() + ".mtl" << endl;

		ofstream mofs(directory / (stem.string() + ".mtl"));
		stringstream mss;

		int vertexOffset = 0;
		for (auto& geometry : geometries)
		{
			ss << "o " << geometry->GetName() << endl;

			auto material = dynamic_cast<HeMaterialSingleTexture*>(geometry->GetMaterial());
			if (material != nullptr)
			{
				ss << "usemtl " << material->GetName() << endl;

				auto texture = material->GetTexture();
				auto image = texture->GetImage();
				image->Write((directory / (image->GetName() + ".png")).string());

				mss << "newmtl " << material->GetName() << endl;
				mss << "map_Kd " << image->GetName() + ".png" << endl;
			}

			auto nov = (int)geometry->GetVertexCount();
			for (int i = 0; i < nov; i++)
			{
				auto& v = geometry->GetVertex(i);
				ss << "v " << v << endl;
			}

			auto nouv = (int)geometry->GetUVCount();
			for (int i = 0; i < nouv; i++)
			{
				auto& uv = geometry->GetUV(i);
				ss << "vt " << uv << endl;
			}

			auto non = (int)geometry->GetNormalCount();
			for (int i = 0; i < non; i++)
			{
				auto& vn = geometry->GetNormal(i);
				ss << "vn " << vn << endl;
			}

			bool has_uv = geometry->GetUVCount() != 0;
			bool has_vn = geometry->GetNormalCount() != 0;

			auto nof = (int)geometry->GetIndexCount() / 3;
			for (int i = 0; i < nof; i++)
			{
				GLuint face[3] = { (GLuint)i * 3 + 1 + vertexOffset, (GLuint)i * 3 + 2 + vertexOffset, (GLuint)i * 3 + 3 + vertexOffset };

				if (has_uv && has_vn)
				{
					ss << "f "
						<< face[0]<< "/" << face[0] << "/" << face[0] << " "
						<< face[1]<< "/" << face[1] << "/" << face[1] << " "
						<< face[2]<< "/" << face[2] << "/" << face[2] << endl;
				}
				else if (has_uv)
				{
					ss << "f "
						<< face[0] << "/" << face[0] << " "
						<< face[1] << "/" << face[1] << " "
						<< face[2] << "/" << face[2] << endl;
				}
				else if (has_vn)
				{
					ss << "f "
						<< face[0] << "//" << face[0] << " "
						<< face[1] << "//" << face[1] << " "
						<< face[2] << "//" << face[2] << endl;
				}
				else
				{
					ss << "f " << face[0] << " " << face[1] << " " << face[2] << endl;
				}
			}

			vertexOffset += nov;
		}

		mofs << mss.rdbuf();
		mofs.close();

		ofs << ss.rdbuf();
		ofs.close();
	}
}
