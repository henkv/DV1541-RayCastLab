#pragma once
#include "Shapes.h"


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
	Vec mid;
	float halfU, halfV, halfW;

	Vec   nU0, nU1, nV0, nV1, nW0, nW1;
	Vec   pU0, pU1, pV0, pV1, pW0, pW1;
	float dU0, dU1, dV0, dV1, dW0, dW1;

public:
	OBB(Vec midPoint, Vec normU, Vec normV, Vec normW, float halfU, float halfV, float halfW, Color color);
	//OBB(Vec midPoint, float halfU, float halfV, float halfW, Color color);

	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;
};