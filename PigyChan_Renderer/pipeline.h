#pragma once
#include "math_Lib.h"
#include "shader.h"
#include "win32.h"

#define PI 3.14159265359
#define EPSILON 1e-5f
#define EPSILON2 1e-5f



const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 800;

//rasterize triangle
void rasterize(Vec4* clipcoord_attri, unsigned char* framebuffer, float* zbuffer, IShader& shader);
void draw_triangles(unsigned char* framebuffer, float* zbuffer, IShader& shader, int nface);