#pragma once

#include "glm\glm.hpp"

#include "ray.h"

class Material;

struct Hit_record {
	float t;
	glm::vec3 p;
	glm::vec3 normal;
	Material *mat_ptr;
};

class Hitable
{
public:
	Hitable();
	~Hitable();
	virtual bool hit(const Ray &r, float t_min, float t_max, Hit_record &rec) const = 0;
};