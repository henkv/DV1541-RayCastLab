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