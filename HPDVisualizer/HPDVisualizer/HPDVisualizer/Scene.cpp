#include "Scene.h"


Scene::Scene(const char* filename)
{
    axesOn();
    W = 600;
    H = 600;
    backgroundColor[0] = 0.0;
    backgroundColor[1] = 0.0;
    backgroundColor[2] = 0.0;
    backgroundColor[3] = 1.0;

    reader.read(filename);
}

Scene::~Scene() 
{
}

void Scene::display() 
{
    light.setLighting();
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
 	glColor3f(1, 1, 1);

    if (drawAxesOn) 
		drawAxes();

    FrameState* frame = reader.get_next_frame();

    if (frame != 0) 
	{
        //unsigned int nObjects = frame->get_num_objects();

		for (unsigned int i = 0; i < reader.objectTypes.size(); i++) 
		{
			ObjectState* o = frame->get_object_id(reader.objectStates[i]->get_object_id());

			if (!o)
				o = reader.objectStates[i];

			ObjectType* type = reader.objectTypes[i];

            if (o != 0) 
			{
				switch (type->type)
				{
				case 0:
					Cube(o->x(), o->y(), o->z(), type->dimensions[0], type->dimensions[1], type->dimensions[2], o->a_r(), o->x_r(), o->y_r(), o->z_r());
					break;
				}

            }
        }
    }
}


void Scene::drawAxes()
{
	double len=1;
 	glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(len,0.0,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,len,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,0.0,len);
    glEnd();
    //  Label axes
    glRasterPos3d(len,0.0,0.0);
    Print("X");
    glRasterPos3d(0.0,len,0.0);
    Print("Y");
    glRasterPos3d(0.0,0.0,len);
    Print("Z");
}


void Scene::Cube(double x, double y, double z, double xLength, double yLength, double zLength, double th, double rx, double ry, double rz)
{
	glPushMatrix();
	// Transform 
	glTranslated(x,y,z);
	glRotated(th,rx,ry,rz);
   	glScaled(xLength/2,yLength/2,zLength/2);
	glBegin(GL_QUADS);
	// +x side
   	glNormal3f( 1, 0, 0);
	glVertex3d(1,1,1);
	glVertex3d(1,1,-1);
	glVertex3d(1,-1,-1);
	glVertex3d(1,-1,1);
	// +z side
  	glNormal3f( 0, 0, 1);
	glVertex3d(1,1,1);
	glVertex3d(-1,1,1);
	glVertex3d(-1,-1,1);
	glVertex3d(1,-1,1);
	// -x side
   	glNormal3f( -1, 0, 0);
	glVertex3d(-1,1,1);
	glVertex3d(-1,1,-1);
	glVertex3d(-1,-1,-1);
	glVertex3d(-1,-1,1);
	// -z side
  	glNormal3f( 0, 0, -1);
	glVertex3d(1,1,-1);
	glVertex3d(-1,1,-1);
	glVertex3d(-1,-1,-1);
	glVertex3d(1,-1,-1);
	// Top
   	glNormal3f( 0, 1, 0);
	glVertex3d(1,1,1);
	glVertex3d(-1,1,1);
	glVertex3d(-1,1,-1);
	glVertex3d(1,1,-1);
	// Bottom
	glNormal3f( 0, -1, 0);
	glVertex3d(1,-1,1);
	glVertex3d(-1,-1,1);
	glVertex3d(-1,-1,-1);
	glVertex3d(1,-1,-1);
	glEnd();
	glPopMatrix();
}

