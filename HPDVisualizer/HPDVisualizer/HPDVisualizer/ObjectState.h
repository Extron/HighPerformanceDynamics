#pragma once

#include <fstream>

using namespace std;

#define UNSIGNEDSHORT sizeof(unsigned short)
#define UNSIGNED sizeof(unsigned int)
#define UNSIGNEDLONG sizeof(unsigned long long) //The long datatype is not garanteed to be 8 bytes, and on many platforms, it is only 4.  Use long long instead.
#define SHORT sizeof(short)
#define INT sizeof(int)
#define LONG sizeof(long)
#define DOUBLE sizeof(double)

class ObjectState 
{
public:
    static const unsigned short objectLengthInBytes = UNSIGNED + 7 * DOUBLE;

    ObjectState();
    ~ObjectState();

    unsigned int get_object_id() { return objectID; }
    double x() { return xPos; }
    double y() { return yPos; }
    double z() { return zPos; }
    double x_r() { return xRot; }
    double y_r() { return yRot; }
    double z_r() { return zRot; }
    double a_r() { return aRot; }

    void read(ifstream& fin);

private:
    unsigned int objectID;
    double xPos;
    double yPos;
    double zPos;
    double xRot;
    double yRot;
    double zRot;
    double aRot;
};
