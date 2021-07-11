#include <Helium/Graphics/Geometry/HePlane.h>

namespace ArtificialNature {

	HePlane::HePlane(const string& name, float columnLength, float rowLength, unsigned int colomns, unsigned int rows, HePlaneType type)
		: HeGeometry(name), columnLength(columnLength), rowLength(rowLength), columns(colomns), rows(rows), type(type)
	{
	}

	HePlane::~HePlane()
	{
	}

	void HePlane::Initialize()
	{
		float columnUnit = columnLength / (float)columns;
		float rowUnit = rowLength / (float)rows;
		float halfColumnLength = columnLength * 0.5f;
		float halfRowLength = rowLength * 0.5f;

		for (GLuint r = 0; r < rows; r++)
		{
			for (GLuint c = 0; c < columns; c++)
			{
				float row = rowUnit * (float)r;
				float column = columnUnit * (float)c;

				switch (type)
				{
				case HePlaneType::XY:
					AddVertex(glm::vec3(column, row, 0.0f));
					break;
				case HePlaneType::YX:
					AddVertex(glm::vec3(row, column, 0.0f));
					break;
				case HePlaneType::YZ:
					AddVertex(glm::vec3(0.0f, column, row));
					break;
				case HePlaneType::ZY:
					AddVertex(glm::vec3(0.0f, row, column));
					break;
				case HePlaneType::XZ:
					AddVertex(glm::vec3(column, 0.0f, row));
					break;
				case HePlaneType::ZX:
					AddVertex(glm::vec3(row, 0.0f, column));
					break;
				default:
					break;
				}

				AddIndex(r * columns + c);
				AddIndex(r * columns + c + 1);
				AddIndex((r + 1) * columns + c);

				AddIndex((r + 1) * columns + c);
				AddIndex(r * columns + c + 1);
				AddIndex((r + 1) * columns + c + 1);

				AddColor(glm::vec4(1, 1, 1, 1));

				AddUV(glm::vec2((float)columns / (float)c, (float)rows / (float)r));
			}
		}

		HeGeometry::Initialize();
	}

	void HePlane::Terminate()
	{
		HeGeometry::Terminate();
	}

}
