#include <Helium/Core/HeTime.h>

namespace ArtificialNature {

	time_point<system_clock> HeTime::Now()
	{
		return system_clock::now();
	}

	double HeTime::DeltaNano(const time_point<system_clock>& t)
	{
		return double(duration_cast<nanoseconds>(system_clock::now() - t).count());
	}

	double HeTime::DeltaMicro(const time_point<system_clock>& t)
	{
		return double(duration_cast<nanoseconds>(system_clock::now() - t).count()) / 1000.0;
	}

	double HeTime::DeltaMili(const time_point<system_clock>& t)
	{
		return double(duration_cast<nanoseconds>(system_clock::now() - t).count()) / 1000000.0;
	}

}
