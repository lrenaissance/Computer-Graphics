#include "raytrace.h"
#include <math.h>

using namespace raytraceData;

raytrace::raytrace(int w, int h)
{
	/* low-level graphics setup */
	lowlevel.initCanvas(w, h);

	/* raytracer setup */
	initCamera(w, h);
	initScene();
}

raytrace::~raytrace()
{
	if (s1 != NULL) delete s1;
	if (c1 != NULL) delete c1;
	if (viewpoint != NULL) delete viewpoint;
}

GLubyte* raytrace::display(void)
{	
	drawScene();  /* draws the picture in the canvas */ 

	return lowlevel.flushCanvas();  /* draw the canvas to the OpenGL window */	
}

void raytrace::initScene()
{
	s1 = makeSphere(0.0, 0.0, -2.0, 0.25);
	s1->m = shader.makeMaterial(0.8, 0.1, 0.15, 0.3);

	//cylinder 추가
	c1 = makeCylinder(-0.25, -0.1, -2,5.0,0.0,0.0,0.001,0.3); //x=-0.25
	c1->m = shader.makeMaterial(0.0, 0.4, 0.6,0.3);

	tracer.s1 = s1;

	//cylinder 추가
	tracer.c1 = c1;
}

void raytrace::initCamera(int w, int h)
{
	viewpoint = makePoint(0.0, 0.0, 0.0, 1.0);
	pnear = -1.0;
	fovx = M_PI / 6;
	/* window dimensions */
	width = w;
	height = h;
}

void raytrace::drawScene(void) //ray trace하는 곳
{
	//모든 px에 대해서 ray를 결정하고 각 px에 대해서 ray 쏘기
	int i, j;
	GLfloat imageWidth;

	/* declare data structures on stack to avoid malloc & free */
	point worldPix;  /* current pixel in world coordinates */
	point direction;
	ray r;
	color c;

	/* z and w are constant for all pixels */
	worldPix.w = 1.0;
	worldPix.z = pnear;

	r.start = &worldPix;
	r.end = &direction;

	imageWidth = 2 * pnear * tan(fovx / 2);

	/* trace a ray for every pixel */
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {

			/* find position of pixel in world coordinates */
			/* y position = (pixel height/middle) scaled to world coords */
			worldPix.y = (j - (height / 2)) * imageWidth / width;
			/* x position = (pixel width/middle) scaled to world coords */
			worldPix.x = (i - (width / 2)) * imageWidth / width;

			/* find direction */
			/* note: direction vector is NOT NORMALIZED */
			//ray 방향 결정
			calculateDirection(viewpoint, &worldPix, &direction);

			/* Find color for pixel */
			//계산 방향으로 ray 쏘기 -> color가 c에 저장
			rayColor(&r, &c);
			/* write the pixel! */
			lowlevel.drawPixel(i, j, c.r, c.g, c.b);
		}
	}
}

point* raytrace::makePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	point* p;
	/* allocate memory */	
	p = new point();
	/* put stuff in it */
	p->x = x; p->y = y; p->z = z; p->w = w;
	return (p);
}

sphere* raytrace::makeSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r) {
	sphere* s;
	/* allocate memory */
	s = new sphere();

	/* put stuff in it */
	s->c = makePoint(x, y, z, 1.0);   /* center */
	s->r = r;   /* radius */
	s->m = NULL;   /* material */
	return(s);
}

cylinder* raytrace::makeCylinder(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat xn, GLfloat yn, GLfloat zn, GLfloat r, GLfloat h) {
	cylinder* c;
	c = new cylinder();
	
	c->c = makePoint(x1, y1, z1, 1.0);
	c->h = h;
	c->r = r;
	c->n = makePoint(xn, yn, zn, 1.0);

	c->m = NULL;
	return c;
}

/* returns the color seen by ray r in parameter c */
//r이라는 ray를 쏘았을 때 ray 컬러를 계산해서 return
void raytrace::rayColor(ray* r, color* c) { //여길 중점적으로 수정(raycolor, trace, shade)
	//step 4: rayColor recursive하게 구현하기
	point p;  /* first intersection point */
	vector n;
	material* m;

	p.w = 0.0;  /* inialize to "no intersection" */
	tracer.trace(r, &p, &n, &m); //trace class의 trace func: ray 날라가서 어떤 물체와 만나고 그 물체의 normal 계산하는 함수
	//trace내에 다른 물체와 intersec 구하는 것 추가(step2)

	if (p.w != 0.0) {
		shader.shade(&p, &n, m, c);  /* do the lighting calculations */ //diffuse, spec shader 내에서 구현(step1)
		//shadow ray: shade 내에서 구현(step3)
	}
	else {             /* nothing was hit */
		c->r = 0.0;
		c->g = 0.0;
		c->b = 0.0; 
	}
}

/* vector from point p to point q is returned in v */
void raytrace::calculateDirection(point* p, point* q, point* v) {
	float length;

	v->x = q->x - p->x;
	v->y = q->y - p->y;
	v->z = q->z - p->z;
	/* a direction is a point at infinity */
	v->w = 0.0;
}
