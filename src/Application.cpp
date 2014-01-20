#include "Application.h"

Application::Application(){
    eyeX = 0;
    eyeZ = 0;
    relativeX = 0;
    wireframe = false;
}

Application::~Application(){

}

void Application::Input(unsigned char key, int x, int y){
    if(key == 119){
        wireframe = (!wireframe);
    }else if(key == 27){
        glutExit();
    }
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

int LightPos[4] = {0,0,0,1};
void Application::Draw(){
    
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //wireframe mode
    if(wireframe){
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    }else{
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
    
    //projection matrice setup
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, 800/800, 1.f, 100.f);
    
    //modify object model view matrice to rotate
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, 0, eyeZ, 0, 0, 0, 0, 1, 0);

    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, 0, cubeVerticesStrip);
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 26);
    
    glutSwapBuffers();
}

void Application::Update(double deltaTime){
    relativeX += (((float)deltaTime*50.f)/800)*3.14f;
    if(relativeX > 3.14f){
        relativeX -= 3.14f;
    }

    eyeX = 0 + 5*sin(relativeX);
    eyeZ = 0 + 5*cos(relativeX);

    glutPostRedisplay();
}