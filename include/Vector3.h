#ifndef _VECTOR3_H
#define _VECTOR3_H
#include <cmath>

class Vector3{
	public:
		Vector3();
		Vector3(Vector3 *);
		Vector3(const Vector3&);
		Vector3(float, float, float);
		Vector3(int, int, int);
		~Vector3();
		
		float X;
		float Y;
        float Z;

		int Distance(Vector3 *);
		float Length();
		Vector3 Normalize();
		Vector3 operator- (Vector3);
		Vector3 operator+ (Vector3);
		Vector3 operator+= (Vector3);
		Vector3 operator* (Vector3);
		Vector3 operator* (float);
        Vector3 operator* (int);
};
#endif
