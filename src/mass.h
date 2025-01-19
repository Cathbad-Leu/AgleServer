
#ifndef _MASS_H
#define _MASS_H

#include "math3d.h"

extern float g_FrameInterval;

///////////////////////////////// CMASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	Un oggetto che rappresenta la massa
/////
///////////////////////////////// CMASS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
class CMass{

	public:

		float m;				// The mass value
		CVector3 Position;		// Position in space
		CVector3 Velocity;		// Velocity
		CVector3 Force;		// Force applied on this mass at an instance

		CMass() {
			m = 1.0;							// The mass value
			Position = CVector3(0.0, 0.0, 0.0);	// Position in space
			Velocity = CVector3(0.0, 0.0, 0.0);	// Velocity
			Force    = CVector3(0.0, 0.0, 0.0);		// Force applied on this mass at an instance
		}
	
		CMass(float m) {
			m = m;
			Position = CVector3(0.0, 0.0, 0.0);	// Position in space
			Velocity = CVector3(0.0, 0.0, 0.0);	// Velocity
			Force    = CVector3(0.0, 0.0, 0.0);		// Force applied on this mass at an instance
		}

		void InitMass() {
			Force = CVector3(0.0, 0.0, 0.0);
		}
	
		void ApplyForce(CVector3 force){
			Force = Force + force; 			// The external force is added to the force of the mass
		}
	
		void UpdateMass() {
			// Change in velocity is added to the velocity.
			// The change is proportinal with the acceleration (force / m) and change in time
			Velocity = ( (Force / m)*g_FrameInterval)+Velocity;
			// Change in position is added to the position.
			// Change in position is velocity times the change in time
			Position = Position  + (Velocity*g_FrameInterval);
		}
};


#endif


