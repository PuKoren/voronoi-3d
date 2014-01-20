#include "Cube.h"

Cube::Cube(){
    delaunayPointsCount = 180;
}

Cube::~Cube(){

}

void Cube::Init(){
    glGenBuffers(1, &VertexArrayID);
    glBindBuffer(GL_ARRAY_BUFFER, VertexArrayID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    RandomizePoints();
}

void Cube::Draw(){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VertexArrayID);

    glVertexAttribPointer(0,// attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 12*3); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);

    glBegin(GL_POINTS);
    for(unsigned int i = 0; i < this->delaunayPoints.size(); i ++){
        glVertex3f(this->delaunayPoints[i].X, this->delaunayPoints[i].Y, this->delaunayPoints[i].Z);
    }
    glEnd( );


}

void Cube::Update(double deltaTime){

}

void Cube::RandomizePoints(){
    std::vector<Vector3> bounds = this->GetBoundaries();
    std::default_random_engine generator;
    generator.seed(time(NULL));
    std::uniform_real_distribution<float> distributionX(bounds[0].X, bounds[1].X);
    std::uniform_real_distribution<float> distributionY(bounds[0].Y, bounds[1].Y);
    std::uniform_real_distribution<float> distributionZ(bounds[0].Z, bounds[1].Z);

    for(int i = 0; i < this->delaunayPointsCount; i ++){
        delaunayPoints.push_back(Vector3(distributionX(generator), distributionY(generator), distributionZ(generator)));
    }
}

std::vector<Vector3> Cube::GetBoundaries(){
    std::vector<Vector3> bounds;
    bounds.push_back(Vector3());
    bounds.push_back(Vector3());

    for(int i = 0; i < 12*3; i += 3){
        if(bounds[0].X > g_vertex_buffer_data[i]){
            bounds[0].X = g_vertex_buffer_data[i];
        }else if(bounds[1].X < g_vertex_buffer_data[i]){
            bounds[1].X = g_vertex_buffer_data[i];
        }
    }
    for(int i = 1; i < 12*3; i += 3){
        if(bounds[0].Y > g_vertex_buffer_data[i]){
            bounds[0].Y = g_vertex_buffer_data[i];
        }else if(bounds[1].Y < g_vertex_buffer_data[i]){
            bounds[1].Y = g_vertex_buffer_data[i];
        }
    }
    for(int i = 2; i < 12*3; i += 3){
        if(bounds[0].Z > g_vertex_buffer_data[i]){
            bounds[0].Z = g_vertex_buffer_data[i];
        }else if(bounds[1].Z < g_vertex_buffer_data[i]){
            bounds[1].Z = g_vertex_buffer_data[i];
        }
    }
    return bounds;
}
