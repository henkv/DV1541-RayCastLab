#include "Shapes.h"


Color Shape::shade(Vec& light, const Vec& cam, Ray& ray, HitData& h)
{
	Color out;
	Color globalLight = { 255, 255, 255 };
	Color ambientLight = { 50, 50, 50 };
	Vec p, l;
	float diff, div = 1.f / 255.f;
	float r, g, b;

	l = light - ray(h.t);
	l.Normalize();

	diff = l.Dot(h.lastNormal);
	diff = diff < 0 ? 0 : diff;

	r = globalLight.r * h.color.r * div * diff + ambientLight.r * h.color.r * div;
	g = globalLight.g * h.color.g * div * diff + ambientLight.g * h.color.g * div;
	b = globalLight.b * h.color.b * div * diff + ambientLight.b * h.color.b * div;

	out.r = r > 255 ? 255 : r;
	out.g = g > 255 ? 255 : g;
	out.b = b > 255 ? 255 : b;

	return out;
}