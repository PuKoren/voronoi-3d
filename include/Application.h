#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "GameObject.h"
#include "Cube.h"

#define M_PI        3.14159265358979323846264338327950288   /* pi */

class Application: public IGameObject{
private:
    float eyeX;
    float eyeZ;
    float relativeX;
    bool wireframe;
    Cube cube;
    float radius;
    float turnSpeed;
    float zoomSpeed;
public:
    Application();
    ~Application();
    virtual void Init();
    virtual void Update(double deltaTime);
    virtual void Draw();
    void Zoom(int direction);
    void Input(unsigned char key, int x, int y);
};

#endif