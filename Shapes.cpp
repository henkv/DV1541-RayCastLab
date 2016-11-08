#include "Shapes.h"


//Color Shape::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
//{
//	Color out;
//	Vec globalLight = { 100, 100, 100 };
//	Color ambientLight = { 5, 5, 5 };
//
//	Vec hit = r(h.t);
//	Vec hitLight = light - hit;
//	hitLight.Normalize();
//
//	float diff = h.lastNormal.Dot(hitLight);
//	out.r = (globalLight.x / 255) * h.color.r * diff;
//	out.g = (globalLight.y / 255) * h.color.g * diff;
//	out.b = (globalLight.z / 255) * h.color.b * diff;
//
//	return out + ambientLight;
//}