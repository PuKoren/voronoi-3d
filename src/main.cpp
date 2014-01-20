#include <GL\glew.h>
#include <GL\freeglut.h>
#include "Application.h"
int previousTime = 0;
int width, height = 800;
Application a;
void Input(unsigned char key, int x, int y);
void Draw();
void Update();
void Reshape (int w, int h);

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);

    glutCreateWindow("Voronoi 3D");
    glewInit();
    glutDisplayFunc(Draw);
    glutIdleFunc(Update);
    glutKeyboardFunc(Input);
    glutReshapeFunc(Reshape);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, width/height, 1.f, 100.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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

void Reshape (int w, int h){
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(45.f, w/h, 1.f, 100.f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}
