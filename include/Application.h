#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <GL\glew.h>
#include <GL\freeglut.h>
#include "GameObject.h"

class Application: public IGameObject{
private:
    float eyeX;
    float eyeZ;
    float relativeX;
public:
    Application();
    ~Application();
    virtual void Update(double deltaTime);
    virtual void Draw();
};


#ifndef ATTRIB_VERTEX
#define ATTRIB_VERTEX 0
#endif

#endif