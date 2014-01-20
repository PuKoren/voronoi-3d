#include "Application.h"

Application::Application(){
}

Application::~Application(){

}

void Application::Draw(){
    glClear (GL_COLOR_BUFFER_BIT);

    glutSwapBuffers();
}

void Application::Update(double deltaTime){
    glutPostRedisplay();
}