#include <GL\glew.h>
#include <GL\freeglut.h>
#include "Application.h"
int previousTime = 0;
Application a;
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

    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return EXIT_SUCCESS;
}

void Update(){
    int deltaTime = (glutGet(GLUT_ELAPSED_TIME) - previousTime);
    previousTime += deltaTime;
    a.Update(deltaTime/1000.f);
}

void Draw(){
    a.Draw();
}