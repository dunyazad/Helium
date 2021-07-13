#include <Helium/Graphics/Geometry/HePlaneGeometry.h>

namespace ArtificialNature {

	HePlaneGeometry::HePlaneGeometry(const string& name, float columnLength, float rowLength, unsigned int colomns, unsigned int rows, HePlaneType type)
		: HeGeometry(name), columnLength(columnLength), rowLength(rowLength), columns(colomns), rows(rows), type(type)
	{
	}

	HePlaneGeometry::~HePlaneGeometry()
	{
	}

	void HePlaneGeometry::Initialize()
	{
		float columnUnit = columnLength / (float)columns;
		float rowUnit = rowLength / (float)rows;
		float halfColumnLength = columnLength * 0.5f;
		float halfRowLength = rowLength * 0.5f;

		for (GLuint r = 0; r <= rows; r++)
		{
			for (GLuint c = 0; c <= columns; c++)
			{
				float row = rowUnit * (float)r;
				float column = columnUnit * (float)c;

				switch (type)
				{
				case HePlaneType::XY:
					AddVertex(glm::vec3(column - halfColumnLength, row - halfRowLength, 0.0f));
					break;
				case HePlaneType::YX:
					AddVertex(glm::vec3(row - halfRowLength, column - halfColumnLength, 0.0f));
					break;
				case HePlaneType::YZ:
					AddVertex(glm::vec3(0.0f, column - halfColumnLength, row - halfRowLength));
					break;
				case HePlaneType::ZY:
					AddVertex(glm::vec3(0.0f, row - halfRowLength, column - halfColumnLength));
					break;
				case HePlaneType::XZ:
					AddVertex(glm::vec3(column - halfColumnLength, 0.0f, row - halfRowLength));
					break;
				case HePlaneType::ZX:
					AddVertex(glm::vec3(row - halfRowLength, 0.0f, column - halfColumnLength));
					break;
				default:
					break;
				}

				if (r < rows && c < columns)
				{
					AddIndex(r * (columns + 1) + c);
					AddIndex(r * (columns + 1) + c + 1);
					AddIndex((r + 1) * (columns + 1) + c);

					AddIndex((r + 1) * (columns + 1) + c);
					AddIndex(r * (columns + 1) + c + 1);
					AddIndex((r + 1) * (columns + 1) + c + 1);
				}

				AddColor(glm::vec4(1, 1, 1, 1));

				AddUV(glm::vec2((float)c / (float)columns, (float)r / (float)rows));
			}
		}

		HeGeometry::Initialize();
	}

	void HePlaneGeometry::Terminate()
	{
		HeGeometry::Terminate();
	}

}
