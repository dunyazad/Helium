#include "HeThickLines.h"

namespace ArtificialNature {

	HeThickLines::HeThickLines()
		: HeGeometry()
	{
		drawingMode = GL_LINE_STRIP;
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

}
