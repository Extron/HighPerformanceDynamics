#ifndef SCENE_H
#define SCENE_H

#include "graphicslib.h"
#include "Texture.h"
#include "Light.h"
#include "TimeSeries.h"
#include "FrameState.h"
#include "ObjectState.h"

class Scene
{
  public:
    Scene(const char* filename);
    ~Scene();

    void axesOn() { drawAxesOn = true; }
    void axesOff() { drawAxesOn = false; }
    void toggleAxes() { drawAxesOn = !drawAxesOn; }
    void display();

private:
    int W;
    int H;
    bool drawAxesOn;
    float backgroundColor[4];
    Light light;
    TimeSeries reader;
 
    void drawScene();
    void drawAxes();

    void Cube(double x, double y, double z, double xLength, double yLength, double zLength, double th, double rx, double ry, double rz);
};

#endif
