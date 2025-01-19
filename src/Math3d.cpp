
#include "math3d.h"

/////////////////////////////////////// VECTOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This calculates a vector between 2 points and returns the result
/////
/////////////////////////////////////// VECTOR \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
CVector3 Vector(CVector3 vPoint1, CVector3 vPoint2)
{
    CVector3 vVector;                           // The variable to hold the resultant vector

    vVector.x = vPoint1.x - vPoint2.x;          // Subtract point1 and point2 x's
    vVector.y = vPoint1.y - vPoint2.y;          // Subtract point1 and point2 y's
    vVector.z = vPoint1.z - vPoint2.z;          // Subtract point1 and point2 z's

    return vVector;                             // Return the resultant vector
}

/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This returns a perpendicular vector from 2 given vectors by taking the cross product.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
    CVector3 vNormal;

    // Calculate the cross product with the non communitive equation
    vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
    vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
    vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

    // Return the cross product
    return vNormal;
}

/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This returns the magnitude of a vector
/////
/////////////////////////////////////// MAGNITUDE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
float Magnitude(CVector3 vNormal)
{
    // Here is the equation:  magnitude = sqrt(V.x^2 + V.y^2 + V.z^2) : Where V is the vector
    return (float)sqrt( (vNormal.x * vNormal.x) + (vNormal.y * vNormal.y) +(vNormal.z * vNormal.z) );
}

/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////   This returns a normalize vector (A vector exactly of length 1)
/////
/////////////////////////////////////// NORMALIZE \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
CVector3 Normalize(CVector3 vVector)
{
    // Get the magnitude of our normal
    float magnitude = Magnitude(vVector);

    // Now that we have the magnitude, we can divide our vector by that magnitude.
    // That will make our vector a total length of 1.
    vVector = vVector / magnitude;

    // Finally, return our normalized vector
    return vVector;
}

