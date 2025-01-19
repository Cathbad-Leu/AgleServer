#ifndef _MATH3D_H
#define _MATH3D_H

#include <math.h>

// This is our basic 3D point/vector class
class CVector3 {

	public:

		float x, y, z;
		// A default constructor
		CVector3() {}
		// This is our constructor that allows us to initialize our data upon creating an instance
		CVector3(float X, float Y, float Z){
			x = X; y = Y; z = Z;
		}
	    // Here we overload the + operator so we can add vectors together
		CVector3 operator+(CVector3 Vector)
		{
			// Return the added vectors result.
			return CVector3(Vector.x + x, Vector.y + y, Vector.z + z);
		}
		// Here we overload the - operator so we can subtract vectors
		CVector3 operator-(CVector3 Vector){
			// Return the subtracted vectors result
			return CVector3(x - Vector.x, y - Vector.y, z - Vector.z);
		}
		// Here we overload the * operator so we can multiply by scalars
		CVector3 operator*(float num){
			// Return the scaled vector
			return CVector3(x * num, y * num, z * num);
		}
		// Here we overload the / operator so we can divide by a scalar
		CVector3 operator/(float num){
			// Return the scale vector
			return CVector3(x / num, y / num, z / num);
		}
};

// This is our basic 2D point class.  This will be used to store the UV coordinates.
class CVector2 {

	public:
		float x, y;
		// A default constructor
		CVector2() {}
		// This is our constructor that allows us to initialize our data upon creating an instance
		CVector2(float X, float Y)
		{
			x = X; y = Y;
		}
		// Here we overload the + operator so we can add vectors together
		CVector2 operator+(CVector2 Vector)
		{
			// Return the added vectors result.
			return CVector2(Vector.x + x, Vector.y + y);
		}
		// Here we overload the - operator so we can subtract vectors
		CVector2 operator-(CVector2 Vector)
		{
			// Return the subtracted vectors result
			return CVector2(x - Vector.x, y - Vector.y);
		}
		// Here we overload the * operator so we can multiply by scalars
		CVector2 operator*(float num)
		{
			// Return the scaled vector
			return CVector2(x * num, y * num);
		}
		// Here we overload the / operator so we can divide by a scalar
		CVector2 operator/(float num)
		{
			// Return the scale vector
			return CVector2(x / num, y / num);
		}
};

float		Magnitude(CVector3 vNormal);
CVector3	Normalize(CVector3 Vector);
CVector3	Cross(CVector3 Vector1, CVector3 Vector2);
CVector3	Vector(CVector3 vPoint1, CVector3 vPoint2);


#endif
