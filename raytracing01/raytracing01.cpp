#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include "glm/glm.hpp"

#if defined(_WIN32)
#include "windows.h"
#endif

#include "camera.h"
#include "hitable.h"
#include "hitablelist.h"
#include "material.h"
#include "sphere.h"
#include "utils.h"

const uint32_t width = 1280;
const uint32_t height = 720;
const uint32_t numSamples = 64;
const bool anti_aliasing = true;

Hitable_List world;

glm::vec3 color(Ray &ray, int32_t depth) {
	const float epsilon = 0.001f;
	Hit_record rec;
	if (world.hit(ray, epsilon, FLT_MAX, rec)) {
		Ray scattered;
		glm::vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(ray, rec, attenuation, scattered)) {
			return attenuation * color(scattered, depth + 1);
		}
		else {
			return glm::vec3(0.0f);
		}
	}
	else {
		glm::vec3 unit_direction = glm::normalize(ray.direction());
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);;
	}
}

void base_scene() 
{
	world.hitables = {
		new Sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(glm::vec3(0.5f, 0.5f, 0.5f))),
		new Sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(glm::vec3(0.8f, 0.8f, 0.0f))),
		new Sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f)),
		new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f,new Dielectric(1.5f)),
		new Sphere(glm::vec3(-1.0f, 0.0f, -1.0f), -0.45f,new Dielectric(1.5f))
	};
}

void random_scene() {
	int32_t n = 500;
	world.hitables.push_back(new Sphere(glm::vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(glm::vec3(0.5f))));
	int32_t i = 0;
	for (int32_t a = -11; a < 11; a++) {
		for (int32_t b = -11; b < 11; b++) {
			float choose_mat = drand48();
			glm::vec3 center = glm::vec3(a + 0.9f * drand48(), 0.2, b + 0.9f*drand48());
			if (glm::length(center - glm::vec3(4.0f, 0.2f, 0.0f)) > 0.9f) {
				if (choose_mat < 0.8f) {
					world.hitables.push_back(
						new Sphere(center, 0.2f, new Lambertian(glm::vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48())))
					);
				}
				else if (choose_mat < 0.95) {
					world.hitables.push_back(
						new Sphere(center, 0.2f, new Metal(glm::vec3(0.5f*(1.0f + drand48()), 0.5f*(1.0f + drand48()), 0.5f*(1.0f + drand48())), 0.5f * drand48()))
					);
				}
				else {
					world.hitables.push_back(
						new Sphere(center, 0.2f, new Dielectric(1.5f))
					);
				}
			}
		}
	}

	world.hitables.push_back(new Sphere(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f)));
	world.hitables.push_back(new Sphere(glm::vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian({ 0.4f, 0.2f, 0.1f })));
	world.hitables.push_back(new Sphere(glm::vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal({ 0.7f, 0.6f, 0.5f }, 0.0f)));
}

int main()
{
	const int32_t scene = 1;

	glm::vec3 lookfrom;
	glm::vec3 lookat;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	float fov;
	float dist_to_focus;
	float aperture;

	if (scene == 0) {
		lookfrom = glm::vec3(-1.0f, 1.0f, 1.0f);
		lookfrom = glm::vec3(0.0f, 1.0f, 1.0f);
		lookat = glm::vec3(0.0f, 0.0f, -1.0f);
		fov = 90.0f;
		dist_to_focus = glm::length(lookfrom - lookat);
		aperture = 2.0f;
		base_scene();
	}
	else {
		lookfrom = glm::vec3(13.0f, 2.0f, 3.0f);
		lookat = glm::vec3(0.0f, 0.0f, 0.0f);
		dist_to_focus = 10.0f;
		aperture = 0.1f;
		fov = 20.0f;
		random_scene();
	}

	Camera camera(lookfrom, lookat, up, fov, float(width) / float(height), aperture, dist_to_focus);

	glm::ivec3* image_data = new glm::ivec3[width * height];

	auto tStart = std::chrono::high_resolution_clock::now();

	#pragma omp parallel for schedule(dynamic,1)
	for (int32_t y = height - 1; y >= 0; y--) {
		for (int32_t x = 0; x < width; x++) {
			glm::vec3 col = glm::vec3(0.0f);
			if (anti_aliasing) {
				for (int32_t s = 0; s < numSamples; s++) {
					float u = float(x + drand48()) / float(width);
					float v = float(y + drand48()) / float(height);
					Ray r = camera.get_ray(u, v);
					col += color(r, 0);
				}
				col /= float(numSamples);
			}
			else {
				float u = float(x) / float(width);
				float v = float(y) / float(height);
				Ray r = camera.get_ray(u, v);
				col += color(r, 0);
			}

			col = glm::vec3(sqrt(col.r), sqrt(col.g), sqrt(col.b));
			int idx = (height - y - 1) * width + x;
			image_data[idx] = (col * 255.99f);
		}
	}

	auto tEnd = std::chrono::high_resolution_clock::now();
	auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();

	std::cout << "Image generated in " << tDiff << " ms";

	std::ofstream file("output.ppm", std::ios::out | std::ios::binary);
	file << "P6\n" << width << " " << height << "\n" << 255 << "\n";
	for (uint32_t i = 0; i < width * height; i++) {
		glm::ivec3 icol = image_data[i];
		file << char(icol.r) << char(icol.g) << char(icol.b);
	}
	file.close();

	delete image_data;

#if defined(_WIN32)
	ShellExecute(GetDesktopWindow(), _T("open"), _T("output.ppm"), NULL, NULL, SW_SHOWNORMAL);
#endif

	std::getchar();

	return 0;
}

