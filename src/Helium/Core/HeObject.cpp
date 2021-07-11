#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	long HeObject::instancCount = 0;

	HeObject::HeObject(const string& name)
		: instanceID(NewID()), name(name)
	{
	}

	HeObject::~HeObject()
	{
	}

	long HeObject::NewID()
	{
		return instancCount++;
	}

}
