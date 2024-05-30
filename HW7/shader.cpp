#include "shader.h"
#include "tracer.h"
#include <math.h>

using namespace raytraceData;

shader::shader()
{
}

shader::~shader()
{
}

material* shader::makeMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat amb) {
	material* m;

	/* allocate memory */
	m = new material();

	/* put stuff in it */
	m->c.r = r;
	m->c.g = g;
	m->c.b = b;
	m->amb = amb;
	m->diff = 0.8;
	m->spec = 0.8;
	return(m);
}

/* LIGHTING CALCULATIONS */

/* shade */
/* color of point p with normal vector n and material m returned in c */
void shader::shade(point* p, vector* n, material* m, color* c) { //local illum

	//illum 계산(illumination model 여기 들어감)

	//light define 후 phong illum model

	light* l = setLight(10, -10, 30);

	float amb_g = 1.0; //추가함

	/* so far, just finds ambient component of color */ //amb만 사용됨(spec, diff 추가)
	c->r = m->amb * m->c.r * amb_g;
	c->g = m->amb * m->c.g * amb_g;
	c->b = m->amb * m->c.b * amb_g;

	//diffuse
	point* tmp = new point;
	tmp->x = l->pos.x - p->x;
	tmp->y = l->pos.y - p->y;
	tmp->z = l->pos.z - p->z;
	normalize(tmp);

	float diffuse = (find_cos(tmp, n) * m->diff * l->intensity);

	//specular
	float cos_phi = 2 * pow(find_cos(tmp, n), 2) - 1;
	if (cos_phi <= 0) {
		cos_phi = 0;
	}

	float specular = pow(cos_phi, l->shine) * m->spec * l->intensity;

	c->r += (diffuse + specular) * m->c.r;
	c->g += (diffuse + specular) * m->c.g;
	c->b += (diffuse + specular) * m->c.b;

	//c->r += vis_i * (diffuse + specular) * m->c.r;
	//c->g += vis_i * (diffuse + specular) * m->c.g;
	//c->b += vis_i * (diffuse + specular) * m->c.b;

	/* clamp color values to 1.0 */
	if (c->r > 1.0) c->r = 1.0;
	if (c->g > 1.0) c->g = 1.0;
	if (c->b > 1.0) c->b = 1.0;
}

void shader::normalize(point* n){
	float dist = sqrt(n->x * n->x + n->y * n->y + n->z * n->z);
	n->x /= dist;
	n->y /= dist;
	n->z /= dist;
}

light* shader::setLight(GLfloat x, GLfloat y, GLfloat z) {
	light* l=new light;
	l->pos.x = x;
	l->pos.y = y;
	l->pos.z = z;
	l->intensity = 1.0; 
	l->shine = 50;

	return l;
}

float shader::dot(vector* a, vector* b){
	float v = a->x * b->x + a->y * b->y + a->z * b->z;
	return v;
}

float shader::dist(point p, point l_pos) {
	float dist = sqrt((p.x - l_pos.x)* (p.x - l_pos.x) + (p.y - l_pos.y) * (p.y - l_pos.y) +(p.z - l_pos.z) * (p.z - l_pos.z));
	return dist;
}

float shader::find_cos(point* p, point* n) {
	point origin = { 0,0,0 };
	float cos_val = dot(p, n) / (dist(*p, origin)*dist(*n,origin));
	if (cos_val < 0) {
		cos_val=0;
	}
	return cos_val;
}

int shader::vis(point* p, light* l, material* m) {
	tracer t;
	ray *r=new ray;
	r->start = p;
	r->end->x = l->pos.x - p->x;
	r->end->y = l->pos.y - p->y;
	r->end->z = l->pos.z - p->z;
	r->end->w = 1.0;
	point* n;
	n->x = n->y = n->z = n->w = 0.0;

	if (t.trace(r, p, n, &m)==0) {
		return 0;
	}
	return 1;
}