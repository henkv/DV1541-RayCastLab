#pragma once
#include "Shapes.h"


class MyPlane : public Shape
{
private:
	Vec n;
	Vec p;
	float d;
	float k;

public:
	MyPlane(Vec normal, float distance, Color color);


	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;

};

class MySphere : public Shape
{
private:
	Vec p;
	float r;
	float r2;

public:
	MySphere(Vec center, float radius, Color color);

	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;
};

class MyTriangle : public Shape
{
private:
	Vec p0, p1, p2, e0, e1, n;
public:
	MyTriangle(Vec p0, Vec p2, Vec p3, Color color);

	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;
	virtual Vec normal(Vec & point) override;

};