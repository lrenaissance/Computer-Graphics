#pragma once

#include <glad/glad.h>
#include <stdlib.h> 
#include "raytraceData.h"

class shader {
public: 
	shader();
	~shader();

	raytraceData::material* makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb);
	void shade(raytraceData::point* p, raytraceData::vector* n, raytraceData::material* m, raytraceData::color* c);

	//Ãß°¡
	void shader::normalize(raytraceData::vector* n);
	raytraceData::light* shader::setLight(GLfloat x, GLfloat y, GLfloat z);
	float shader::dot(raytraceData::vector* a, raytraceData::vector* b);
	float shader::dist(raytraceData::point p, raytraceData::point l_pos);
	float shader::find_cos(raytraceData::point* p, raytraceData::point* n);
	int shader::vis(raytraceData::point* p, raytraceData::light* l, raytraceData::material* m);
};
