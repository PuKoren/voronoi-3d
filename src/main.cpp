#include <GL\freeglut.h>

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    
    glutCreateWindow("Voronoi 3D");
    glutMainLoop();
	return EXIT_SUCCESS;
}