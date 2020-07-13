#pragma once

#include "maths.h"

namespace maths
{
    template<class num>
    struct Mat4
    {
        num content[16];

        num* operator[](int index)
        {
            return this->content + index * 4;
        }

        void applyToVector(num* vector)
        {
            // Copying the vector
            num vectorCopy[4];
            for (int i = 0; i < 4; ++i)
                vectorCopy[i] = vector[i];

            // Calculating the multiplication
            for (int i = 0; i < 4; ++i)
            {
                vector[i] = dotProduct<num>(4, this->operator[](i), vectorCopy);
            }
        }

        void loadIdentity()
        {
            // Initialising everything to 0

            for (int i = 0; i < 16; ++i)
                this->content[i] = 0;

            // Adding the necessary 1's
            for (int i = 0; i < 4; ++i)
                (*this)[i][i] = 1;

        }

        void scale(num scalar)
        {
            for (int i = 0; i < 16; ++i)
                this->content[i] *= scalar;
        }

        void scale(num* scalingVector4)
        {
            Mat4<num> scalingMatrix;
            scalingMatrix.loadIdentity();

            for (int i = 0; i < 4; ++i)
            {
                scalingMatrix[i][i] = scalingVector4[i];
            }

            (*this) = (*this) * scalingMatrix;
        }

        void scale(Vec3<num> scalingVector)
        {
            num scalingVector4[4] = { scalingVector.x ,scalingVector.y, scalingVector.z, num(1) };
            this->scale(scalingVector4);
        }

        void rotate(num x, num y, num z)
        {
            Mat4<num> xRotationMatrix, yRotationMatrix, zRotationMatrix;
            xRotationMatrix.loadIdentity();
            yRotationMatrix.loadIdentity();
            zRotationMatrix.loadIdentity();

            // Initialising the x Matrix
            xRotationMatrix[1][1] = cosd(x);
            xRotationMatrix[2][2] = cosd(x);
            xRotationMatrix[1][2] = -sind(x);
            xRotationMatrix[2][1] = sind(x);

            // Initialising the y Matrix
            yRotationMatrix[2][2] = cosd(y);
            yRotationMatrix[0][0] = cosd(y);
            yRotationMatrix[2][0] = -sind(y);
            yRotationMatrix[0][2] = sind(y);

            // Initialising the z Matrix
            zRotationMatrix[0][0] = cosd(z);
            zRotationMatrix[1][1] = cosd(z);
            zRotationMatrix[0][1] = -sind(z);
            zRotationMatrix[1][0] = sind(z);

            (*this) = (((*this) * xRotationMatrix) * yRotationMatrix) * zRotationMatrix;
        }

        void rotate(Vec3<num> rotationVector)
        {
            this->rotate(rotationVector.x, rotationVector.y, rotationVector.z);
        }

        void translate(Vec3<num> translation)
        {
            Mat4<num> translationMatrix;
            translationMatrix.loadIdentity();

            translationMatrix[0][3] = translation.x;
            translationMatrix[1][3] = translation.y;
            translationMatrix[2][3] = translation.z;

            (*this) = (*this) * translationMatrix;
        }

        std::string toString()
        {
            std::string result;

            for (int i = 0; i < 16; ++i)
            {
                result += std::to_string(this->content[i]) + " ";
                if (i % 4 == 3) result += "\n";
            }

            return result;
        }

        Mat4<num> operator*(Mat4<num> other)
        {
            Mat4<num> result;

            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                {
                    num otherVector[4];

                    for (int k = 0; k < 4; ++k)
                    {
                        otherVector[k] = other[k][j];
                    }

                    result[i][j] = dotProduct(4, (*this)[i], otherVector);
                }

            return result;
        }

        Vec3<num> operator * (Vec3<num> source)
        {
            num resultVector[4] = { source.x, source.y, source.z, num(1) };
            this->applyToVector(resultVector);
            return { resultVector[0], resultVector[1], resultVector[2] };
        }
    };
}