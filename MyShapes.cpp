#include "MyShapes.h"

Plane::Plane(Vec normal, float distance, Color color)
{
	this->n/*ormal*/ = normal;
	this->d/*istance*/ = distance;
	this->c/*olor*/ = color;
}

Plane::~Plane()
{
}

void Plane::test(Ray & ray, HitData & hit)
{
	Vec norm = normal(ray.o);
	float dot = ray.d.Dot(norm);

	if (dot != 1)
	{
		float t = -(
			(norm.x*ray.o.x + norm.y*ray.o.y + norm.z*ray.o.z + d) / 
			(norm.x*ray.d.x + norm.y*ray.d.y + norm.z*ray.d.z)
		);

		hit.color = c;
		hit.lastNormal = norm;
		hit.lastShape = this;
		hit.t = t;
	}
}

Vec Plane::normal(Vec & point)
{
	return n;
}
