#pragma once

#include <math.h>


namespace randomFloat
{
    float mod(float a, float b)
    {
        return float(int(a) % int(b)) + (a - round(a)) * (b - round(b));
    }

    float randomNumber(float x)
    {
        if (x > -2.f && x < 2.f) return 4.2f;
        float result = 0.f;
        for (float i = 0.f; i < 32.f; i++)
        {
            float currentPow = pow(3.141f, i);
            result += mod(currentPow + 1.f, x) - mod(currentPow + 2.f, x) + mod(currentPow + 3.f, x) - mod(currentPow + 4.f, x) + mod(currentPow + 5.f, x);
        }
        return result / x;
    }
}