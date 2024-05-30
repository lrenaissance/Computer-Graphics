#include "tracer.h"

#include <math.h>

using namespace raytraceData;

tracer::tracer():
	s1(NULL), c1(NULL)
{
}

tracer::~tracer()
{
}

/* point on ray r parameterized by t is returned in p */
void tracer::findPointOnRay(ray* r, float t, point* p) {
	p->x = r->start->x + t * r->end->x;
	p->y = r->start->y + t * r->end->y;
	p->z = r->start->z + t * r->end->z;
	p->w = 1.0;
}

/* raySphereIntersect */
/* returns TRUE if ray r hits sphere s, with parameter value in t */
int tracer::raySphereIntersect(ray* r, sphere* s, float* t) { //이미 구현 완료
	point p;   /* start of transformed ray */
	float a, b, c;  /* coefficients of quadratic equation */
	float D;    /* discriminant */
	point* v;

	/* transform ray so that sphere center is at origin */
	/* don't use matrix, just translate! */
	p.x = r->start->x - s->c->x;
	p.y = r->start->y - s->c->y;
	p.z = r->start->z - s->c->z;
	v = r->end; /* point to direction vector */


	a = v->x * v->x + v->y * v->y + v->z * v->z;
	b = 2 * (v->x * p.x + v->y * p.y + v->z * p.z);
	c = p.x * p.x + p.y * p.y + p.z * p.z - s->r * s->r;

	D = b * b - 4 * a * c;

	if (D < 0) {  /* no intersection */
		return (FALSE);
	}
	else {
		D = static_cast<float>(sqrt(D));
		/* First check the root with the lower value of t: */
		/* this one, since D is positive */
		*t = (-b - D) / (2 * a);
		/* ignore roots which are less than zero (behind viewpoint) */
		if (*t < 0) {
			*t = (-b + D) / (2 * a);
		}
		if (*t < 0) { return(FALSE); }
		else return(TRUE);
	}
}

int tracer::rayCylinderIntersect(ray* r, cylinder* cy, float* t) {
	point o;
	o.x = r->start->x - cy->c->x;
	o.y = r->start->y - cy->c->y;
	o.z = r->start->z - cy->c->z;

	point d;
	d.x = r->end->x;
	d.y= r->end->y;
	d.z = r->end->z;

	point p;

	// 무한한 높이의 원기둥. xz 교점 찾아서 y 구하기
	// at^2 + bt + c = 0
	float a = o.x * o.x + o.z * o.z;
	float b = 2 * (d.x * o.x + d.z * o.z);
	float c = o.x * o.x + o.z * o.z - cy->r * cy->r;

	// 판별식
	float det = b * b - 4 * a * c;
	if (det < 0) {
		return FALSE;
	}

	// 근
	float p1 = (-b + sqrt(det)) / (2 * a);
	float p2 = (-b - sqrt(det)) / (2 * a);

	if (p1 < p2) { // 근 두 개일 경우 먼저 만나는 애 기준
		*t = p1;
	}
	else {
		*t = p2;
	}

	p.y = o.y + d.y * (*t);

	if (p.y <= cy->c->y + cy->h && p.y >= cy->c->y) { // 원기둥 높이 사이에 교점 y 있어야함
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/* normal vector of s at p is returned in n */
/* note: dividing by radius normalizes */
void tracer::findSphereNormal(sphere* s, point* p, vector* n) {
	n->x = (p->x - s->c->x) / s->r;
	n->y = (p->y - s->c->y) / s->r;
	n->z = (p->z - s->c->z) / s->r;
	n->w = 0.0;
}

void tracer::findCylinderNormal(cylinder* c, point* p, vector* n) {
	n->x = (p->x - c->c->x);
	n->y = 0.0;
	n->z = (p->z - c->c->z);
	n->w = 0.0;
	float dist = sqrt(n->x * n->x + n->y * n->y + n->z * n->z);
	n->x /= dist;
	n->z /= dist;
}

/* trace */
/* If something is hit, returns the finite intersection point p,
   the normal vector n to the surface at that point, and the surface
   material m. If no hit, returns an infinite point (p->w = 0.0) */
point* tracer::trace(ray* r, point* p, vector* n, material** m) {
	float t = 0;     /* parameter value at first hit */
	int hit_s = FALSE;
	int hit_c = FALSE;

	hit_s = raySphereIntersect(r, s1, &t); //현재 sphere밖에 없어서 ray와 sphere 만나는지 이걸로 계산
	//물체 추가시 이거랑 비슷한 애 추가해야함
	hit_c = rayCylinderIntersect(r, c1, &t);

	if (hit_s) { //ray와 만난다
		*m = s1->m;
		findPointOnRay(r, t, p); //만나는 곳
		if (t <= 0.001) {
			p->w = 0.0;
			return p;
		}
		findSphereNormal(s1, p, n); //normal 계산
	}
	else if (hit_c) { //ray와 만난다
		*m = c1->m;
		findPointOnRay(r, t, p); //만나는 곳
		if (t <= 0.001) {
			p->w = 0.0;
			return p;
		}
		findCylinderNormal(c1, p, n); //normal 계산
	}
	else {
		/* indicates nothing was hit */
		p->w = 0.0;
		return p;
	}
	return p;
}