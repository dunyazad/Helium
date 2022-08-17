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
	};

}
