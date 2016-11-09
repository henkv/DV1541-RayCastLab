#pragma once
#include "Shapes.h"

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

		float d1, d2, d3;
		float d4, d5, d6;
		float d7, d8, d9;

		d1 = m[1][1] * m[2][2] - m[1][2] * m[2][1];
		d2 = m[1][0] * m[2][2] - m[1][2] * m[2][0];
		d3 = m[1][0] * m[2][1] - m[1][1] * m[2][0];
		d4 = m[0][1] * m[2][2] - m[0][2] * m[2][1];
		d5 = m[0][0] * m[2][2] - m[0][2] * m[2][0];
		d6 = m[0][0] * m[2][1] - m[0][1] * m[2][0];
		d7 = m[0][1] * m[1][2] - m[0][2] * m[1][1];
		d8 = m[0][0] * m[1][2] - m[0][2] * m[1][0];
		d9 = m[0][0] * m[1][1] - m[0][1] * m[1][0];

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


class Plane : public Shape
{
private:
	Vec n;
	Vec p;
	float d;
	float k;

public:
	Plane(Vec normal, float distance, Color color);


	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;

};

class Sphere : public Shape
{
private:
	Vec p;
	float r;
	float r2;

public:
	Sphere(Vec center, float radius, Color color);

	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;
};

class Triangle : public Shape
{
private:
	Vec p0, p1, p2, e0, e1, n;
public:
	Triangle(Vec p0, Vec p2, Vec p3, Color color);

	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;

};

class OBB : public Shape
{
private:
	Vec m, uN, vN, wN;
	Vec uO, vO, wO;
	float uH, vH, wH;
	float uK, vK, wK;

public:
	OBB(Vec midPoint, Vec normU, Vec normV, Vec normW, float halfU, float halfV, float halfW, Color color);
	//OBB(Vec midPoint, float halfU, float halfV, float halfW, Color color);

	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;
};