#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

class IGameObject{
public:
    virtual ~IGameObject() {};
    virtual void Draw() = 0;
    virtual void Init() = 0;
    virtual void Update(double elapsedTime) = 0;
};

#endif