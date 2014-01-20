#ifndef _APPLICATION_H
#define _APPLICATION_H
#include <GL\freeglut.h>
#include "GameObject.h"

class Application: public IGameObject{
private:
    
public:
    Application();
    ~Application();
    virtual void Update(double deltaTime);
    virtual void Draw();
};

#endif