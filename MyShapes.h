#pragma once
#include "Shapes.h"


class Plane : public Shape
{
private:
	Vec n;
	float d;

public:
	Plane(Vec normal, float distance, Color color);
	virtual ~Plane();


	// Inherited via Shape
	virtual void test(Ray & ray, HitData & hit) override;

	virtual Vec normal(Vec & point) override;

};

