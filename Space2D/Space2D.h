#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define TAO 2*M_PI

#define ANGLE_RIGHT 0.0
#define ANGLE_UP TAO/4.0
#define ANGLE_LEFT TAO/2.0
#define ANGLE_DOWN 3.0*TAO/4.0


namespace Space2D {	
	/*space containing code related to 2D space algorithm 
	* such as coordinate plane geometry, vector math, 
	* and certain physics calculations */

	class Point;	//class containing X and Y components
	class Vector;	//class containing Magnitude and Direction components
	class Velocity;	//class inheriting Vector and Point
}