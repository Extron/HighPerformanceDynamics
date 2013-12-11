#include "ObjectType.h"

ObjectType::ObjectType()
{
}

void ObjectType::read(ifstream& fin)
{
	char size;
	fin.read((char*)&size, 1);
	fin.read((char*)&type, 1);

	switch (type)
	{
	case 0:
		double length, width, depth;
		fin.read((char*)&length, sizeof(double));
		fin.read((char*)&width, sizeof(double));
		fin.read((char*)&depth, sizeof(double));

		dimensions[0] = length;
		dimensions[1] = width;
		dimensions[2] = depth;
		break;

	case 1:
		double radius;
		fin.read((char*)&radius, sizeof(double));

		dimensions[0] = dimensions[1] = dimensions[2] = radius;
		break;

	case 2:
		double r, height;
		fin.read((char*)&r, sizeof(double));
		fin.read((char*)&height, sizeof(double));

		dimensions[0] = dimensions[1] = r;
		dimensions[2] = radius;
		break;
	}
}