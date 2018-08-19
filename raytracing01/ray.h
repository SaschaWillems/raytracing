#pragma once

#include "glm\glm.hpp"

class Ray {
public:
	glm::vec3 A, B;
	Ray();
	~Ray();
	Ray(const glm::vec3 &A, const glm::vec3 &B);
	glm::vec3 origin() const;
	glm::vec3 direction() const;
	glm::vec3 point_at_parameter(float t) const;
};