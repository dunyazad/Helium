#include <Helium/Core/HeObject.h>

namespace ArtificialNature {

	long HeObject::instancCount = 0;

	HeObject::HeObject()
		: instanceID(NewID())
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
