#pragma once

#ifdef HELIUM_EXPORTS
#define HELIUM_API __declspec(dllexport)
#else
#define HELIUM_API __declspec(dllimport)
#endif


#include <iostream>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <deque>
#include <vector>

#include <algorithm>
#include <chrono>
#ifndef _M_CEE
#include <thread>
#endif
using namespace std;

#include <Windows.h>

#include <GL/glew.h>
#ifdef _DEBUG
#pragma comment(lib, "glew/glew32d.lib")
#else
#pragma comment(lib, "glew/glew32.lib")
#endif

#include <gl/gl.h>
#include <gl/glu.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <glm/glm.hpp>

typedef unsigned char byte;


#define LOAD_MODULE(X) ::LoadLibrary(X)

#define HELIUM (ArtificialNature::Helium*)(::GetProcAddress(LOAD_MODULE(L"Helium.dll"), "CreateInstance"))();
