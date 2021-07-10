#pragma once

#include <Helium/Core/HeliumCommon.h>

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

	template<typename T>
	class HeCallback
	{
	public:
		HeCallback(T t)
		{
			this->t = t;
		}

		T t;
	};
}
