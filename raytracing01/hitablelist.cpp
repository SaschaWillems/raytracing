#include "stdafx.h"
#include "hitablelist.h"


bool Hitable_List::hit(const Ray & r, float t_min, float t_max, Hit_record & rec) const
{
	Hit_record temp_rec;
	bool hit_anything(false);
	double closest_so_far = t_max;
	for (auto &hitable : hitables) {
		if (hitable->hit(r, t_min, closest_so_far, temp_rec)) {
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}
