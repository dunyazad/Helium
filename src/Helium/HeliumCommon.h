#pragma once

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
#include <glm/glm.hpp>






void _CheckGLError(const char* file, int line);
#define CheckGLError() _CheckGLError(__FILE__, __LINE__)
