#include "oldFunctions.h"

void oldf::glu::Perspective(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble fW, fH;
    fH = tan((fovY / 2) / 180 * pi) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

bool oldf::glu::Project(GLdouble objx, GLdouble objy, GLdouble objz, GLdouble* modelview, GLdouble* projection, GLint* viewport, float& windowCoordinateX, float& windowCoordinateY, float& windowCoordinateZ)
{
    // Transformation vectors
    GLdouble fTempo[8];
    // Modelview transform
    fTempo[0] = modelview[0] * objx + modelview[4] * objy + modelview[8] * objz + modelview[12]; // w is always 1
    fTempo[1] = modelview[1] * objx + modelview[5] * objy + modelview[9] * objz + modelview[13];
    fTempo[2] = modelview[2] * objx + modelview[6] * objy + modelview[10] * objz + modelview[14];
    fTempo[3] = modelview[3] * objx + modelview[7] * objy + modelview[11] * objz + modelview[15];
    // Projection transform, the final row of projection matrix is always [0 0 -1 0]
    // so we optimize for that.
    fTempo[4] = projection[0] * fTempo[0] + projection[4] * fTempo[1] + projection[8] * fTempo[2] + projection[12] * fTempo[3];
    fTempo[5] = projection[1] * fTempo[0] + projection[5] * fTempo[1] + projection[9] * fTempo[2] + projection[13] * fTempo[3];
    fTempo[6] = projection[2] * fTempo[0] + projection[6] * fTempo[1] + projection[10] * fTempo[2] + projection[14] * fTempo[3];
    fTempo[7] = -fTempo[2];
    // The result normalizes between -1 and 1
    if (fTempo[7] == 0.0) // The w value
        return 0;
    fTempo[7] = 1.0 / fTempo[7];
    // Perspective division
    fTempo[4] *= fTempo[7];
    fTempo[5] *= fTempo[7];
    fTempo[6] *= fTempo[7];
    // Window coordinates
    // Map x, y to range 0-1
    windowCoordinateX = (float)(fTempo[4] * 0.5 + 0.5) * viewport[2] + viewport[0];
    windowCoordinateY = (float)(fTempo[5] * 0.5 + 0.5) * viewport[3] + viewport[1];
    // This is only correct when glDepthRange(0.0, 1.0)
    windowCoordinateZ = (float)(1.0 + fTempo[6]) * 0.5;	// Between 0 and 1
    return 1;
}

maths::Vec2<float> oldf::glu::simpleProjectRelative(maths::Vec3<float> point3d)
{
    GLdouble projectionMatrix[16];
    GLdouble modelviewMatrix[16];
    GLint viewport[4];

    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    float x, y, z;
    oldf::glu::Project(point3d.x, point3d.y, point3d.z, modelviewMatrix, projectionMatrix, viewport, x, y, z);

    maths::Vec2<float> result(
        maths::mapFromRangeToRange(x, float(viewport[0]), float(viewport[0] + viewport[2]), 0.f, 1.f),
        maths::mapFromRangeToRange(y, float(viewport[1]), float(viewport[1] + viewport[3]), 0.f, 1.f)
    );

    return result;
}