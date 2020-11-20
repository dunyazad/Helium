#include "HeThickLines.h"

namespace ArtificialNature {

	HeThickLines::HeThickLines()
		: HeGeometry()
	{
		drawingMode = GL_LINE_STRIP;
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
