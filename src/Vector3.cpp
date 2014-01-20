#include "Vector3.h"

Vector3::Vector3(Vector3 *v){
	this->X = v->X;
	this->Y = v->Y;
    this->Z = v->Z;
}
Vector3::Vector3(){
	this->X = 0;
	this->Y = 0;
    this->Z = 0;
}
Vector3::Vector3(const Vector3 &v){
	this->X = v.X;
	this->Y = v.Y;
    this->Z = v.Z;
}
Vector3::~Vector3(){

}
Vector3::Vector3(int x, int y, int z = 0){
	this->X = (float)x;
	this->Y = (float)y;
}

Vector3::Vector3(float x, float y, float z = 0.f){
	this->X = x;
	this->Y = y;
    this->Z = z;
}

/*
* Compute the Euclidian distance between two Vector3->
* Distance is sqrt((v1->X - v2->X)² + (v1->Y - v2->Y)²)
*/
int Vector3::Distance(Vector3 *v){
	return (int)sqrt((float)(this->X - v->X)*(this->X - v->X) + (this->Y - v->Y) * (this->Y - v->Y) + (this->Z - v->Z) * (this->Z - v->Z));
}

/*
* Compute the length of a vector
* sqrt(x²+y²)
*/
float Vector3::Length(){
	return sqrt((float)(this->X*this->X + this->Y*this->Y + this->Z * this->Z));
}

/*
* Normalize a vector
* Vector3(X/Length, Y/Length)
*/
Vector3 Vector3::Normalize(){
    float length = this->Length();
    return Vector3(this->X/length, this->Y/length, this->Z/length);
}

Vector3 Vector3::operator-(Vector3 v){
    return Vector3(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}

Vector3 Vector3::operator+(Vector3 v){
    return Vector3(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

Vector3 Vector3::operator+=(Vector3 v){
	return Vector3(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

Vector3 Vector3::operator*(Vector3 v){
    return Vector3(this->X * v.X, this->Y * v.Y, this->Z * v.Z);
}

Vector3 Vector3::operator*(float f){
	return Vector3(this->X * f, this->Y * f, this->Z * f);
}

Vector3 Vector3::operator*(int f){
    return Vector3(this->X * f, this->Y * f, this->Z * f);
}
