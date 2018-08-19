#pragma once

#include <vector>

#include "glm\glm.hpp"
#include "hitable.h"

class Hitable_List : public Hitable {
public:
	std::vector<Hitable*> hitables;
	Hitable_List() {}
	virtual bool hit(const Ray &r, float t_min, float t_max, Hit_record &rec) const;
};
