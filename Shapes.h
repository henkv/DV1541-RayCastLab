#pragma once
#include <math.h>
#include <memory.h>
//#include <assert.h>
#include <algorithm>

typedef unsigned char BYTE;

struct Vec
{
	float	x, y, z;
	Vec() {};
	Vec(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	float Dot(const Vec& v2) { return (x * v2.x) + (y * v2.y) + (z * v2.z); }
	Vec Cross(const Vec& v2)
	{
		/*
			i   j   k
		    x   y   z
			a   b   c

			Vec(y * c - z * b, z * a - x * c, x * b - y * a);
		*/
		return Vec(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x);
	};

	Vec operator-(const Vec& v)
	{
		return Vec(this->x - v.x, this->y - v.y, this->z - v.z);
	}

	Vec operator+(const Vec& v)
	{
		return Vec(this->x + v.x, this->y + v.y, this->z + v.z);
	}

	Vec operator*(const float& f)
	{
		return Vec(this->x * f, this->y * f, this->z * f);
	}

	// useful when comparing lengths! (faster)
	inline float Length2()
	{
		return (x*x + y*y + z*z);
	}

	float Length()
	{
		return sqrt(Length2());
	}

	void Normalize()
	{
		float l = Length();
		x /= l;
		y /= l;
		z /= l;
	}
};

struct Mat
{
	float m[3][3];

	Mat(Vec c0 = { 0,0,0 }, Vec c1 = { 0,0,0 }, Vec c2 = { 0,0,0 })
	{
		m[0][0] = c0.x;	m[0][1] = c1.x; m[0][2] = c2.x;
		m[1][0] = c0.y; m[1][1] = c1.y; m[1][2] = c2.y;
		m[2][0] = c0.z; m[2][1] = c1.z; m[2][2] = c2.z;
	}

	Mat inv()
	{
		Mat o;

		float d1 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
		float d2 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
		float d3 = m[1][0] * m[2][1] - m[1][1] * m[2][0];

		float d4 = m[0][1] * m[2][2] - m[0][2] * m[2][1];
		float d5 = m[0][0] * m[2][2] - m[0][2] * m[2][0];
		float d6 = m[0][0] * m[2][1] - m[0][1] * m[2][0];

		float d7 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
		float d8 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
		float d9 = m[0][0] * m[1][1] - m[0][1] * m[1][0];

		o.m[0][0] =  d1; o.m[0][1] = -d4; o.m[0][2] =  d7;
		o.m[1][0] = -d2; o.m[1][1] =  d5; o.m[1][2] = -d8;
		o.m[2][0] =  d3; o.m[2][1] = -d6; o.m[2][2] =  d9;


		float det = m[0][0] * d1 - m[0][1] * d2 + m[0][2] * d3;

		o = o * (1 / det);

		return o;
	}

	Mat& operator*(float f)
	{
		for (int i = 0, j; i < 3; i++)
		{
			for (j = 0; j < 3; j++)
			{
				m[i][j] *= f;
			}
		}

		return *this;
	}

	Vec operator*(const Vec & v)
	{
		Vec o;

		o.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
		o.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
		o.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;

		return o;
	}
};

struct Ray
{
	Vec	o;
	Vec	d;

	Vec operator()(float t)
	{
		return o + d * t;
	}

	Ray(Vec origin, Vec direction) : o(origin), d(direction) { };
};

struct Color
{
	BYTE	r;
	BYTE	g;
	BYTE	b;

	Color() { memset(&r, 0, 3); }
	Color(BYTE red, BYTE green, BYTE blue) : r(red), g(green), b(blue) {};
	const Color& operator+(const Color& plus) {
		unsigned int rr, gg, bb;
		rr = plus.r + r; 
		if (rr > 255) 
			rr = 255;
		gg = plus.g + g; 
		if (gg > 255) 
			gg= 255;
		bb = plus.b + b; 
		if (bb > 255) 
			bb = 255;
		return Color(rr,gg,bb);
	}
};

class Shape;
struct HitData
{
	float	t;
	Color	color;
	Shape* lastShape = nullptr;
	Vec lastNormal;
	// hit data is -1 (not hit) and black (default color for background)
	HitData() : t(-1.0f), color(0, 0, 0) { };
};

class Shape
{
public:
	Color	c;
	// each subclass, has to implement these two methods!
	virtual void test(Ray& ray, HitData& hit) = 0;
	virtual Vec normal(Vec &point) = 0;

	// Light with (0.5,0.5,0.5) intensity for ambient, diffuse and specular
	// This method is the same for all clases, so implement only for Shape
	virtual Color shade(Vec& light, const Vec& cam, Ray& r, HitData& h);
};


// for each class, add the necessary data that describes the shape,
// implement a constructor, and implement the test function
// EACH CLASS SHOULD INHERIT FROM SHAPE,
class MPlane : public Shape
{
	Vec		n;
	float	d;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	MPlane(Vec normal, float _d, Color color);
};

class MSphere : public Shape
{
	Vec center;
	float radius;
	float radius2;
public:
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	MSphere(Vec _center, float _radius, Color _color);
};

class MTriangle : public Shape
{
	Vec p1, p2, p3, nor;
	Vec edge0, edge1;
	public:
		void test(Ray& ray, HitData& hit);
		Vec normal(Vec &point) { return nor; }
		MTriangle(Vec _p1, Vec _p2, Vec _p3, Color _color);

};

class MOBB : public Shape
{
public:
	Vec Bcenter;
	Vec Bu;
	Vec Bv;
	Vec Bw;

	Vec Pu,Puo;
	Vec Pv,Pvo;
	Vec Pw,Pwo;

	float halfBu;
	float halfBv;
	float halfBw;

	void test(Ray& ray, HitData& hit);
	Vec normal(Vec& point);

	// Center point, lenght U vector, length V vector, length W vector, color

	MOBB(Vec b, Vec b1, Vec b2, Vec b3, float Hu, float Hv, float Hw, Color _color);
	MOBB(Vec b, float Hu, float Hv, float Hw, Color _color);
};