#pragma once

#include <Helium/Core/HeliumCommon.h>

namespace ArtificialNature {

	class HeSettings {
	public:
		string& operator[](const string& key) { return settings[key]; }

	protected:
		map<string, string> settings;
	};

}
