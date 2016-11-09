#include "MyShapes.h"

Plane::Plane(Vec normal, float distance, Color color)
{
	this->n/*ormal*/ = normal;
	this->p/*oint*/ = normal * distance;
	this->k/*onstant*/ = -n.Dot(p);

	this->d/*istance*/ = distance;
	this->c/*olor*/ = color;

	// n.x + n.y + n.z + k = 0;
}

void Plane::test(Ray & r, HitData & hit)
{
	float dot = n.Dot(r.d);

	if (dot != 0)
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

		float t = -(n.Dot(r.o) + k) / dot;

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
	this->r = radius;
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
	*/
	Vec po = r.o - p;
	float a = r.d.Dot(po);
	float b = po.Dot(po) - r2;
	float c = a * a - b;

	if (c > 0)
	{
		float sqrc = sqrt(c);
		float t1 = -a + sqrc;
		float t2 = -a - sqrc;

		if (t1 > t2) std::swap(t1, t2);


		if (t1 > 0 && (t1 < hit.t || hit.t < 0))
		{
			hit.t = t1;
			hit.color = this->c;
			hit.lastNormal = normal(r(t1));
			hit.lastShape = this;
		}
		else if (t2 > 0 && (t2 < hit.t || hit.t < 0))
		{
			hit.t = t2;
			hit.color = this->c;
			hit.lastNormal = normal(r(t2));
			hit.lastShape = this;
		}
	}
	else if (c == 0)
	{
		float t = -a + sqrt(c);

		if (t > 0 && (t < hit.t || hit.t < 0))
		{
			hit.t = t;
			hit.color = this->c;
			hit.lastNormal = normal(r.o);
			hit.lastShape = this;
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
	if (ndotd != 0)
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
	this->m = midPoint;

	this->uN = normU;
	this->vN = normV;
	this->wN = normW;

	this->uH = halfU;
	this->vH = halfV;
	this->wH = halfW;

	this->uO = uN * uH;
	this->vO = vN * vH;
	this->wO = wN * wH;

	this->c = color;
}

void OBB::test(Ray & r, HitData & hit)
{
	float tMin = -INFINITY;
	float tMax = INFINITY;
	float t1, t2, k1, k2;
	Vec p1, p2;

	float uNDd = uN.Dot(r.d);
	if (uNDd != 0)
	{
		float uNDo = uN.Dot(r.o);
		t1 = (uN.Dot(m + uO) - uNDo) / uNDd;
		t2 = (uN.Dot(m - uO) - uNDo) / uNDd;
		if (t1 > t2) std::swap(t1, t2);
		if (t1 > tMin) tMin = t1;
		if (t2 < tMax) tMax = t2;
		if (tMin > tMax) return;
		if (tMax < 0) return;
	}
	else
	{
		p1 = m + uO;
		p2 = m - uO;
		k1 = -uN.Dot(p1);
		k2 = -uN.Dot(p2);
		k1 += uN.Dot(p1 + r.d);
		k2 += uN.Dot(p2 + r.d);

		if (k1 != 0 && k2 != 0) return;
	}

	float vNDd = vN.Dot(r.d);
	if (vNDd != 0)
	{
		float vNDo = vN.Dot(r.o);
		t1 = (vN.Dot(m + vO) - vNDo) / vNDd;
		t2 = (vN.Dot(m - vO) - vNDo) / vNDd;
		if (t1 > t2) std::swap(t1, t2);
		if (t1 > tMin) tMin = t1;
		if (t2 < tMax) tMax = t2;
		if (tMin > tMax) return;
		if (tMax < 0) return;
	}
	else
	{
		p1 = m + vO;
		p2 = m - vO;
		k1 = -vN.Dot(p1);
		k2 = -vN.Dot(p2);
		k1 += vN.Dot(p1 + r.d);
		k2 += vN.Dot(p2 + r.d);

		if (k1 != 0 && k2 != 0) return;
	}

	float wNDd = wN.Dot(r.d);
	if (wNDd != 0)
	{
		float wNDo = wN.Dot(r.o);
		t1 = (wN.Dot(m + wO) - wNDo) / wNDd;
		t2 = (wN.Dot(m - wO) - wNDo) / wNDd;
		if (t1 > t2) std::swap(t1, t2);
		if (t1 > tMin) tMin = t1;
		if (t2 < tMax) tMax = t2;
		if (tMin > tMax) return;
		if (tMax < 0) return;
	}
	else
	{
		p1 = m + wO;
		p2 = m - wO;
		k1 = -wN.Dot(p1);
		k2 = -wN.Dot(p2);
		k1 += wN.Dot(p1 + r.d);
		k2 += wN.Dot(p2 + r.d);

		if (k1 != 0 && k2 != 0) return;
	}

	if (tMin <= tMax)
	{
		if (tMin > 0 && (hit.t > tMin || hit.t < 0))
		{
			hit.t = tMin;
			hit.color = c;
			hit.lastShape = this;
			hit.lastNormal = Vec();
		}
	}	
}

Vec OBB::normal(Vec & point)
{
	Vec op = point - m;
	op.Normalize();

	float big;
	Vec norm;



	return norm;
}
