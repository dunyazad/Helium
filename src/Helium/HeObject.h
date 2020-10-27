#pragma once

#include "HeliumCommon.h"

namespace ArtificialNature {

	class HeObject
	{
	public:
		HeObject();
		~HeObject();

		inline long GetInstanceID() { return instanceID; }
	protected:
		long NewID();

	private:
		static long instancCount;
		long instanceID = -1;
	};

}
