#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include "glm\glm.hpp"

#define drand48() (rand() / (RAND_MAX + 1.0))

inline float squared_length(glm::vec3 v) {
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

inline glm::vec3 random_in_unit_sphere() {
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(drand48(), drand48(), drand48()) - glm::vec3(1.0f);
	} while ((squared_length(p)) >= 1.0f);
	return p;
}

#endif // _UTILS_H_