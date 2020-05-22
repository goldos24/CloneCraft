#pragma once

#include <SFML/OpenGL.hpp>
#include <math.h>

namespace oldf {

	namespace glu
	{

		const GLdouble pi = 3.1415926535897932384626433832795;
		// Replaces gluPerspective. Sets the frustum to perspective mode.
		// fovY     - Field of vision in degrees in the y direction
		// aspect   - Aspect ratio of the viewport
		// zNear    - The near clipping distance
		// zFar     - The far clipping distance

		void Perspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
		{
			GLdouble fW, fH;
			fH = tan((fovY / 2) / 180 * pi) * zNear;
			fH = tan(fovY / 360 * pi) * zNear;
			fW = fH * aspect;
			glFrustum(-fW, fW, -fH, fH, zNear, zFar);
		}
	}

}