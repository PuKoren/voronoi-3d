#include "Application.h"

Application::Application(){
    eyeX = 0;
    eyeZ = 0;
    relativeX = 0;
    wireframe = false;
}

Application::~Application(){

}

void Application::Init(){
    glEnable(GL_DEPTH_TEST);
    cube.Init();
}

void Application::Input(unsigned char key, int x, int y){
    if(key == 119){
        wireframe = (!wireframe);
    }else if(key == 27){
        glutExit();
    }
}

void Application::Draw(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //wireframe mode
    if(wireframe){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }else{
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    
    //modify object model view matrice to rotate with eyeX and eyeZ values
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, 0, eyeZ, 0, 0, 0, 0, 1, 0);
    
    cube.Draw();

    glutSwapBuffers();
}

void Application::Update(double deltaTime){
    relativeX += (((float)deltaTime*50.f)/800)*3.14f;
    if(relativeX > 3.14f){
        relativeX -= 3.14f;
    }

    eyeX = 0 + 5*sin(relativeX);
    eyeZ = 0 + 5*cos(relativeX);

    cube.Update(deltaTime);

    glutPostRedisplay();
}