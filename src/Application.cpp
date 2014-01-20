#include "Application.h"

Application::Application(){
    eyeX = 0;
    eyeZ = 0;
    relativeX = 0;
}

Application::~Application(){

}

const GLfloat cubeVerticesStrip[] = {
        -1,-1,1, 1,-1,1, -1,1,1, 1,1,1,
        1,1,1, 1,-1,1, 1,1,-1, 1,-1,-1,
        1,-1,-1, -1,-1,-1, 1,1,-1, -1,1,-1,
        -1,1,-1, -1,-1,-1, -1,1,1, -1,-1,1,
        -1,-1,1, -1,-1,-1, 1,-1,1, 1,-1,-1,
        1,-1,-1, -1,1,1,
        -1,1,1, 1,1,1, -1,1,-1, 1,1,-1
    };

void Application::Draw(){
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //wireframe
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, 800/800, 1.f, 100.f);

    //drawing cube
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, 0, eyeZ, 0, 0, 0, 0, 1, 0);

    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, cubeVerticesStrip);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 26);
    
    glutSwapBuffers();
}
#include <math.h>
void Application::Update(double deltaTime){
    relativeX += ((deltaTime*50.f)/800)*3.14;
    if(relativeX > 3.14){
        relativeX -= 3.14;
    }
    //float relativeY = (((float)y/(float)WINDOW_HEIGHT))*ESGI_PI;
    eyeX = 0 + 5*sin(relativeX);
    //eyeY = 0 + radius*sin(relativeY);
    eyeZ = 0 + 5*cos(relativeX);

    glutPostRedisplay();
}