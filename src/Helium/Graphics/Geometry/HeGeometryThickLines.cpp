#include <Helium/Graphics/Geometry/HeGeometryThickLines.h>

namespace ArtificialNature {

	HeGeometryThickLines::HeGeometryThickLines(const string& name)
		: HeGeometry(name)
	{
		drawingMode = LineStrip;
		//drawingMode = GL_LINE_STRIP_ADJACENCY;
	}

	HeGeometryThickLines::~HeGeometryThickLines()
	{
	}

	void HeGeometryThickLines::Initialize()
	{
		HeGeometry::Initialize();
	}

	void HeGeometryThickLines::Terminate()
	{
		HeGeometry::Terminate();
	}

	void HeGeometryThickLines::SetThickness(float thickness)
	{
		this->thickness = thickness;

		glLineWidth(thickness);
	}
}
