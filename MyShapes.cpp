#include "MyShapes.h"
#define EPS 0.0001
#define IS_ZERO(f) (abs(f) < EPS)
#define NOT_ZERO(f) (abs(f) > EPS)

Plane::Plane(Vec normal, float distance, Color color)
{
	this->n = normal;
	this->nDp = n.Dot(normal * distance);
	this->c = color;
}

void Plane::test(Ray & r, HitData & hit)
{
	float nDd = n.Dot(r.d);

	if (NOT_ZERO(nDd))
	{
		/*
		n*(p - (o + td)) = 0
		n*p - n*(o + td) = 0
		n*p = n*(o + td)
		n*p = n*o + tn*d
		t = (nDp - n*o) / nDd
		*/

		float t = (nDp - n.Dot(r.o)) / nDd;

		if (t > 0 && (hit.t > t || hit.t < 0))
		{
			hit.t = t;
			hit.color = c;
			hit.lastNormal = n;
			hit.lastShape = this;
		}
	}
}

Vec Plane::normal(Vec & point)
{
	return n;
}

Sphere::Sphere(Vec center, float radius, Color color)
{
	this->c = color;
	this->p = center;
	this->r2 = radius * radius;
}

void Sphere::test(Ray & r, HitData & hit)
{
	/*
		f(x) = ||x - p|| - r = 0
		f(ray) = ||o + td - p|| - r = 0
		f(ray) = (o + td - p) * (o + td - p) = r2
		f(ray) = (o + td - p) * (o + td - p) = r2
		f(ray) = (td + (o - p)) * (td + (o - p)) = r2
		f(ray) = td * td + td * (o - p) + td * (o - p) + (o - p) * (o - p) = r2
		f(ray) = t2 + 2t(d * (o - p)) + (o - p) * (o - p) - r2 = 0
		a = (d * (o - p))
		b = (o - p) * (o - p) - r2
		t2 + 2ta + b = 0
		t = -a +- sqr(a2 - b)
		c = sqr(a2 - b)
	*/
	Vec po = r.o - p;
	if (po.Length2() >= r2)
	{
		float a = r.d.Dot(po);
		float b = po.Dot(po) - r2;
		float c = a * a - b;
		if (c >= 0)
		{
			float sqrc = sqrt(c);
			float t1 = -a + sqrc;
			float t2 = -a - sqrc;
			if (t1 > t2) std::swap(t1, t2);
			if (t1 > 0 && (t1 < hit.t || hit.t < 0))
			{
				hit.t = t1;
				hit.color = this->c;
				hit.lastShape = this;
				hit.lastNormal = normal(r(t1));
			}
		}
	}
}

Vec Sphere::normal(Vec & o)
{
	Vec n = o - p;
	n.Normalize();
	return n;
}

Triangle::Triangle(Vec p0, Vec p1, Vec p2, Color color)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->e0 = p1 - p0;
	this->e1 = p2 - p0;
	this->n = e0.Cross(e1);
	n.Normalize();
	this->c = color;
}

void Triangle::test(Ray & ray, HitData & hit)
{
	float ndotd = n.Dot(ray.d);
	if (NOT_ZERO(ndotd))
	{
		Mat M = { ray.d * -1, e0, e1 };
		Vec p = ray.o - p0;
		Vec tuv = M.inv() * p;
		float t = tuv.x;
		float u = tuv.y;
		float v = tuv.z;

		if (t > 0 && u >= 0 && v >= 0 && (u + v) <= 1)
		{
			if (t < hit.t || hit.t < 0)
			{
				hit.t = t;
				hit.color = this->c;
				hit.lastNormal = n;
				hit.lastShape = this;
			}
		}
	}
}

Vec Triangle::normal(Vec & point)
{
	return n;
}

OBB::OBB(Vec midPoint, Vec normU, Vec normV, Vec normW, float halfU, float halfV, float halfW, Color color)
{
	this->c = color;
	this->m = midPoint;

	this->normals[0] = normU;
	this->normals[1] = normV;
	this->normals[2] = normW;

	this->halfWidths[0] = halfU;
	this->halfWidths[1] = halfV;
	this->halfWidths[2] = halfW;

	this->centers[0] = m + normals[0] * halfWidths[0];
	this->centers[1] = m - normals[0] * halfWidths[0];
	this->centers[2] = m + normals[1] * halfWidths[1];
	this->centers[3] = m - normals[1] * halfWidths[1]; 
	this->centers[4] = m + normals[2] * halfWidths[2];
	this->centers[5] = m - normals[2] * halfWidths[2];

}

OBB::OBB(Vec midPoint, float halfU, float halfV, float halfW, Color color)
	:OBB(midPoint, { 1,0,0 }, { 0,1,0 }, { 0,0,1 }, halfU, halfV, halfW, color)
{}

void OBB::test(Ray & r, HitData & hit)
{
	float tMin = -INFINITY;
	float tMax = INFINITY;
	Vec p = m - r.o;
	
	float e, f, t1, t2;
	for (int i = 0; i <3; i++)
	{
		e = normals[i].Dot(p);
		f = normals[i].Dot(r.d);
		if (NOT_ZERO(f))
		{
			t1 = (e + halfWidths[i]) / f;
			t2 = (e - halfWidths[i]) / f;
			if (t1 > t2) std::swap(t1, t2);
			if (t1 > tMin) tMin = t1;
			if (t2 < tMax) tMax = t2;
			if (tMin > tMax) return;
			if (tMax < 0) return;
		}
		else if ((-e - halfWidths[i]) > 0 || (-e + halfWidths[i]) < 0) return;
	}
	
	float t = tMin > 0 ? tMin : tMax;
	if (t > 0 && (hit.t > t || hit.t < 0))
	{
		hit.t = t;
		hit.color = c;
		hit.lastShape = this;
		hit.lastNormal = normal(r(t));
	}
}

Vec OBB::normal(Vec & point)
{
	Vec n = { 0,0,0 };

	for (int i = 0, i2; i < 3; i++)
	{
		i2 = i * 2;
		if (IS_ZERO(normals[i].Dot(point - centers[i2])))
		{
			n = normals[i];
			break;
		}
		else if (IS_ZERO(normals[i].Dot(point - centers[i2 + 1])))
		{
			n = normals[i] * -1;
			break;
		}
	}

	return n;
}
