#include "stdafx.h"
#include "ray.h"

Ray::Ray() {}

Ray::~Ray() {}

Ray::Ray(const glm::vec3 &A, const glm::vec3 &B) {
	this->A = A;
	this->B = B;
}
glm::vec3 Ray::origin() const {
	return A;
}
glm::vec3 Ray::direction() const {
	return B;
}
glm::vec3 Ray::point_at_parameter(float t) const {
	return A + t * B;
}

