#pragma once

#include <glad/glad.h>

/* constants */
#define TRUE 1
#define FALSE 0

#define M_PI 3.1415926535897932384626433832795029

namespace raytraceData {

	/* data structures */
	typedef struct point {
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat w;
	} point;

	/* a vector is just a point */
	typedef point vector;

	/* a line segment */
	typedef struct segment {
		point* start;
		point* end;
	} segment;

	/* a ray is just a segment with an endpoint at infinity */
	typedef segment ray;

	typedef struct color {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		/* these should be between 0 and 1 */
	} color;

	typedef struct material {
		/* color */
		color c;
		/* ambient reflectivity */
		GLfloat amb;
		GLfloat diff;
		GLfloat spec;
		//GLfloat transparency_m; //0과 1사이
		//GLfloat reflectivity_m; //0과 1사이
	} material;

	typedef struct sphere {
		point* c;  /* center */
		GLfloat r;  /* radius */
		material* m;
	} sphere;

	typedef struct cylinder { //원통
		point* c;  /* center */ //cylinder 윗면
		GLfloat r;  /* radius */
		material* m;
		GLfloat h;
		vector* n;
	};

	///////////////////////////////////////////////////////////////////////////
	//
	//	GLfloat amb_g; //general amb lighting lv
	//	GLfloat amb_m; //amb reflectance of surface
	//	bool vis; //0: p에서 광원 안 보임. 1: p에서 광원 보임
	//	GLfloat theta; //angle between ray(from p to light src) & surface norm 
	//	GLfloat dif_m; //diff reflectance of surface
	//	GLfloat intensity; //intensity of light src
	//	color color_m; //color of the material

	//	GLfloat phi; //angle between r and reflection of the ray(from light src at p)
	//	GLfloat spec_m; //specular reflectance of the surface
	//	GLfloat shine; //how sharp spec is(usually>1)
	//	GLfloat transparency_m; //0과 1사이
	//	ray r_t; //transmitted ray through the surface
	//	ray r_f; //refracted ray by the surface(굴절)
	//	GLfloat reflectivity_m; //0과 1사이
	//

	typedef struct light {
		point pos;//빛 들어오는 방향

		GLfloat intensity;//intensity of light src
		GLfloat shine;//how sharp spec is(usually>1)
	}light;
}