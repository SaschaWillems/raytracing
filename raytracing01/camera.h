#pragma once

#include "glm\glm.hpp"
#include "ray.h"
#include "utils.h"

class Camera {
public:
	glm::vec3 origin;
	glm::vec3 lower_left_corner;
	glm::vec3 horizontal;
	glm::vec3 vertical;
	glm::vec3 u, v, w;
	bool depth_of_field = false;
	float lens_radius;
	Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect, float aperture, float focus_dist);
	Ray get_ray(float s, float t);
};