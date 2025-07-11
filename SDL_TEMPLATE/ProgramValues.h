#pragma once

struct vec4 {
	float r;
	float g;
	float b;
	float a;
};

struct vec3 {
	float x;
	float y;
	float z;
};

namespace ProgramValues {
	extern float x;
	extern float y;
	extern float dimensionScalar;
	extern vec4 v1_color;
	extern vec4 v2_color;
	extern vec4 v3_color;
	extern vec4 v4_color;
	extern int ProgramDimensionX;
	extern int ProgramDimensionY;
}