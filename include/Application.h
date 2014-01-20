#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "GameObject.h"
#include "Cube.h"

class Application: public IGameObject{
private:
    float eyeX;
    float eyeZ;
    float relativeX;
    bool wireframe;
    Cube cube;
public:
    Application();
    ~Application();
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void Draw();
    void Input(unsigned char key, int x, int y);
};

#endif