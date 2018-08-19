#pragma once

#include "utils.h"
#include "ray.h"
#include "hitable.h"

class Material
{
public:
	Material();
	~Material();
	virtual bool scatter(const Ray &r_in, const Hit_record &rec, glm::vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
public:
	glm::vec3 albedo;
	Lambertian(const glm::vec3 &albedo) : albedo(albedo) {};
	virtual bool scatter(const Ray &r_in, const Hit_record &rec, glm::vec3 &attenuation, Ray &scattered) const;
};

class Metal : public Material {
public:
	glm::vec3 albedo;
	float fuzz;
	Metal(const glm::vec3 &albedo, float fuzz) : albedo(albedo) {
		this->fuzz = (fuzz < 1.0f ? fuzz : 1.0f);
	};
	virtual bool scatter(const Ray &r_in, const Hit_record &rec, glm::vec3 &attenuation, Ray &scattered) const;
};

class Dielectric : public Material {
public:
	float ref_idx;
	Dielectric(float ref_idx) : ref_idx(ref_idx) {};
	virtual bool scatter(const Ray &r_in, const Hit_record &rec, glm::vec3 &attenuation, Ray &scattered) const;
};