#pragma once

#include "raytraceData.h"

class tracer {
public:
	tracer();
	~tracer();

	void findPointOnRay(raytraceData::ray* r, float t, raytraceData::point* p);
	int raySphereIntersect(raytraceData::ray* r, raytraceData::sphere* s, float* t);
	void findSphereNormal(raytraceData::sphere* s, raytraceData::point* p, raytraceData::vector* n);
	raytraceData::point* trace(raytraceData::ray* r, raytraceData::point* p, raytraceData::vector* n, raytraceData::material** m);

	//cylinder 추가
	int tracer::rayCylinderIntersect(raytraceData::ray* r, raytraceData::cylinder* c, float* t);
	void tracer::findCylinderNormal(raytraceData::cylinder* c, raytraceData::point* p, raytraceData::vector* n);

	raytraceData::sphere* s1;	   /* the scene: so far, just one sphere */
	raytraceData::cylinder* c1; //cy 추가
};