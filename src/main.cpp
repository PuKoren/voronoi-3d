#include "Application.h"
int previousTime = 0;
Application a;
void Draw();
void Update();

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);

    glutCreateWindow("Voronoi 3D");
    glutDisplayFunc(Draw);
    glutIdleFunc(Update);

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