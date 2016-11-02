#include "MyShapes.h"

MyPlane::MyPlane(Vec normal, float distance, Color color)
{
	this->n/*ormal*/ = normal;
	this->p/*oint*/ = normal * distance;
	this->k/*onstant*/ = -n.Dot(p);

	this->d/*istance*/ = distance;
	this->c/*olor*/ = color;

	// n.x + n.y + n.z + k = 0;
}

void MyPlane::test(Ray & r, HitData & hit)
{
	float dot = n.Dot(r.d);

	if (dot == 0)
	{
		hit.t = -1.f;
	}
	else
	{
		/*
			 | x = r.o.x + r.d.x * t
		ray: | y = r.o.y + r.d.y * t
		     | z = r.o.z + r.d.z * t

		plane: n.x + n.y + n.z + k = 0
		 
		n.x * (r.o.x + r.d.x * t) + n.y * (r.o.y + r.d.y * t) + n.z * (r.o.z + r.d.z * t) + k = 0
		t * (n.x * r.d.x + n.y * r.d.y + n.z * r.d.z) + (n.x * r.o.x +  n.y * r.o.y + n.z * r.o.z) + k = 0
		t * n.Dot(r.d) + n.Dot(r.o) + k = 0
		t * n.Dot(r.d) = -(n.Dot(r.o) + k)
		t = -(n.Dot(r.o) + k) / n.Dot(r.d)
		*/

		float t = -(n.Dot(r.o) + k) / n.Dot(r.d);

		hit.t = t;
		hit.color = c;
		hit.lastNormal = n;
		hit.lastShape = this;
	}
}

Vec MyPlane::normal(Vec & point)
{
	return n;
}

MySphere::MySphere(Vec center, float radius, Color color)
{
	this->c = color;
	this->p = center;
	this->r = radius;
	this->r2 = radius * radius;
}

void MySphere::test(Ray & r, HitData & hit)
{
	/*
		(r.o + r.d * t).Length2 = r2;
		(ro.x^2
		
	*/
}

Vec MySphere::normal(Vec & o)
{
	Vec n = o - p;
	n.Normalize();
	return n;
}
