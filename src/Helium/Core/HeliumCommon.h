#pragma once

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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/intersect.hpp>



#include <ft2build.h>
#include FT_FREETYPE_H 

void _CheckGLError(const char* file, int line);
#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

#define HeDelete(x) if(x != nullptr) { delete x; x = nullptr; }


enum HePlaneType { XY, YX, YZ, ZY, XZ, ZX };