#include "stdafx.h"
#include "material.h"


Material::Material() {}

Material::~Material() {}

bool refract(const glm::vec3 &v, const glm::vec3 n, float ni_over_nt, glm::vec3 &refracted) {
	glm::vec3 uv = glm::normalize(v);
	float dt = glm::dot(uv, n);
	float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1.0f - dt * dt);
	if (discriminant > 0.0f) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}

float schlick(float cosine, float ref_idx) {
	float r0 = (1.0f - ref_idx) / (1.0f + ref_idx);
	r0 = r0 * r0;
	return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}

bool Lambertian::scatter(const Ray &r_in, const Hit_record &rec, glm::vec3 &attenuation, Ray &scattered) const {
	glm::vec3 target = rec.p + rec.normal + random_in_unit_sphere();
	scattered = Ray(rec.p, target - rec.p);
	attenuation = albedo;
	return true;
}

bool Metal::scatter(const Ray & r_in, const Hit_record & rec, glm::vec3 & attenuation, Ray & scattered) const {	
	glm::vec3 reflected = glm::reflect(glm::normalize(r_in.direction()), rec.normal);
	if (fuzz > 0.0f) {
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
	}
	else {
		scattered = Ray(rec.p, reflected);
	}
	attenuation = albedo;
	return true;
}

bool Dielectric::scatter(const Ray & r_in, const Hit_record & rec, glm::vec3 & attenuation, Ray & scattered) const
{
	glm::vec3 outward_normal;
	glm::vec3 reflected = glm::reflect(r_in.direction(), rec.normal);
	float ni_over_nt;
	attenuation = glm::vec3(1.0f);
	glm::vec3 refracted;
	float reflect_prob;
	float cosine;
	if (dot(r_in.direction(), rec.normal) > 0.0f) {
		outward_normal = -rec.normal;
		ni_over_nt = ref_idx;
		cosine = glm::dot(r_in.direction(), rec.normal) / glm::length(r_in.direction());
		cosine = sqrt(1.0f - ref_idx * ref_idx*(1 - cosine * cosine));
	}
	else {
		outward_normal = rec.normal;
		ni_over_nt = 1.0f / ref_idx;
		cosine = -glm::dot(r_in.direction(), rec.normal) / glm::length(r_in.direction());
	}
	if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
		reflect_prob = schlick(cosine, ref_idx);
	}
	else {
		reflect_prob = 1.0f;
	}
	if ((rand() / (RAND_MAX + 1.0)) < reflect_prob) {
		scattered = Ray(rec.p, reflected); 
	}
	else {
		scattered = Ray(rec.p, refracted);
	}
	return true;
}
