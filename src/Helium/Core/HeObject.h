#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeObject
	{
	public:
		HeObject(const string& name);
		~HeObject();

		inline long GetInstanceID() { return instanceID; }
		inline const string& GetName() const { return name; }
	protected:
		long NewID();

	private:
		static long instancCount;
		long instanceID = -1;
		string name;
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
