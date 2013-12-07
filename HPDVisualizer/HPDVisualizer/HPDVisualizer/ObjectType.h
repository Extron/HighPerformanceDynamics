#pragma once
#include <fstream>

using namespace std;

/**
 * This stores details about the type of the object, which is used to determine how to display it.
 */
class ObjectType
{
public:
	ObjectType();
	~ObjectType(void);

	void read(ifstream& fin);

	char type;
	double dimensions[3];
};

