#include "stdafx.h"
#include "camera.h"

#define M_PI 3.14159265358979323846264338327950288

inline glm::vec3 rnd_in_unit_sphere() {
	glm::vec3 p;
	do {
		p = 2.0f * glm::vec3(drand48(), drand48(), 0.0f) - glm::vec3(1.0f, 1.0f, 0.0f);
	} while ((squared_length(p)) >= 1.0f);
	return p;
}

Camera::Camera(glm::vec3 lookfrom, glm::vec3 lookat, glm::vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
{
	float theta = vfov * M_PI / 180;
	float half_height = tan(theta / 2.0f);
	float half_width = aspect * half_height;
	origin = lookfrom;
	w = glm::normalize(lookfrom - lookat);
	u = glm::normalize(glm::cross(vup, w));
	v = glm::cross(w, u);
	lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
	horizontal = 2.0f * half_width * focus_dist * u;
	vertical = 2.0f * half_height * focus_dist * v;
	lens_radius = aperture / 2.0f;
}

Ray Camera::get_ray(float s, float t)
{
	if (depth_of_field) {
		glm::vec3 rd = lens_radius * rnd_in_unit_sphere();
		glm::vec3 offset = u * rd.x + v * rd.y;
		return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}
	else {
		return Ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
	}
}
