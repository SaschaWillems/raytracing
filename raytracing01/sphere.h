#pragma once

#include "glm\glm.hpp"
#include "hitable.h"
#include "material.h"

class Sphere : public Hitable {
public:
	glm::vec3 center;
	float radius;
	Material *mat_ptr;
	Sphere(glm::vec3 center, float radius, Material *material) : center(center), radius(radius), mat_ptr(material) {};
	virtual bool hit(const Ray &r, float t_min, float t_max, Hit_record &rec) const;
};

