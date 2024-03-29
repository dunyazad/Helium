#pragma once

#pragma warning(disable : 4099)
#pragma warning(disable : 4819)

#include <typeinfo>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>

#include <chrono>
#include <mutex>

#include <algorithm>
#include <functional>
#include <memory>
using namespace std;

#include <stb/stb_image.h>

#define __STDC_LIB_EXT1__
#include <stb/stb_image_write.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <tinyply/tinyply.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <json.hpp>
using json = nlohmann::json;

void _CheckGLError(const char* file, int line);
#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

#define HeDelete(x) if(x != nullptr) { delete x; x = nullptr; }


namespace ArtificialNature {
	extern json HeSettings;

	enum HePlaneType { XY, YX, YZ, ZY, XZ, ZX };

	ostream& operator << (ostream& o, const glm::vec2& v);

	ostream& operator << (ostream& o, const glm::vec3& v);

	ostream& operator << (ostream& o, const glm::vec4& v);

	ostream& operator << (ostream& o, const glm::mat3& m);

	ostream& operator << (ostream& o, const glm::mat4& m);

	int safe_stoi(const string& input);

	float safe_stof(const string& input);

	vector<string> split(const string& input, const string& delimiters, bool includeEmptyString = false);

	unsigned int NextPowerOf2(unsigned int n);
}

namespace glm {
	mat4 flip_axes(const mat4& m);
}