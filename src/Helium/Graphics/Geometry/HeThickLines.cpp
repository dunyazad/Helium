#include <Helium/Graphics/Geometry/HeThickLines.h>

namespace ArtificialNature {

	HeThickLines::HeThickLines(const string& name)
		: HeGeometry(name)
	{
		drawingMode = LineStrip;
		//drawingMode = GL_LINE_STRIP_ADJACENCY;
	}

	HeThickLines::~HeThickLines()
	{
	}

	void HeThickLines::Initialize()
	{
		HeGeometry::Initialize();
	}

	void HeThickLines::Terminate()
	{
		HeGeometry::Terminate();
	}

	void HeThickLines::SetThickness(float thickness)
	{
		this->thickness = thickness;

		glLineWidth(thickness);
	}
}
