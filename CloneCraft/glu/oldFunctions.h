#pragma once

#include <SFML/OpenGL.hpp>
#include <math.h>
#include "../maths/maths.h"

namespace oldf {

	namespace glu
	{

		const GLdouble pi = 3.1415926535897932384626433832795;
		// Replaces gluPerspective. Sets the frustum to perspective mode.
		// fovY     - Field of vision in degrees in the y direction
		// aspect   - Aspect ratio of the viewport
		// zNear    - The near clipping distance
		// zFar     - The far clipping distance

		void Perspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);

		bool Project(GLdouble objx, GLdouble objy, GLdouble objz, GLdouble* modelview, GLdouble* projection, GLint* viewport, float& windowCoordinateX, float& windowCoordinateY, float& windowCoordinateZ);

        maths::Vec2<float> simpleProjectRelative(maths::Vec3<float> point3d);
	}

}