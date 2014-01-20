#include <GL\glew.h>
#include <GL\freeglut.h>
#include "Application.h"
int previousTime = 0;
Application a;
void Input(unsigned char key, int x, int y);
void Draw();
void Update();

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);

    glutCreateWindow("Voronoi 3D");
    glewInit();
    glutDisplayFunc(Draw);
    glutIdleFunc(Update);
    glutKeyboardFunc(Input);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glutMainLoop();

    return EXIT_SUCCESS;
}

void Input(unsigned char key, int x, int y){
    a.Input(key, x, y);
}

void Update(){
    int deltaTime = (glutGet(GLUT_ELAPSED_TIME) - previousTime);
    previousTime += deltaTime;
    a.Update(deltaTime/1000.f);
}

void Draw(){
    a.Draw();
}