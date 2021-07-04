#pragma once

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




void _CheckGLError(const char* file, int line);
#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

#define HeDelete(x) if(x != nullptr) { delete x; x = nullptr; }
