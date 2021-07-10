#pragma once

#include <Helium/Core/HeliumCommon.h>

using namespace std::chrono;

namespace ArtificialNature {

	class HeTime
	{
	public:
		static time_point<system_clock> Now();
		static double DeltaNano(const time_point<system_clock>& t);
		static double DeltaMicro(const time_point<system_clock>& t);
		static double DeltaMili(const time_point<system_clock>& t);
	};

}