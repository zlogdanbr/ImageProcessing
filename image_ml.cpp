#include "image_ml.h"
#include <iostream>

void machine_learnit(std::string& f)
{
	mat data;
	data::Load(f, data, true);

	std::cout << data;

}