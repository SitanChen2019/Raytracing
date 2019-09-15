//
// Created by Sitan Chen on 2019/9/6.
//

#ifndef RAYTRACING_RAND_H
#define RAYTRACING_RAND_H
#include <random>
#include "Math.h"

class Random01
{
public:
    Random01(REAL offset = 0, unsigned seed = 0)
            :m_generator(seed)
            ,m_offset(offset)
    {}

    REAL getRandom() {
        return m_distribution(m_generator) + m_offset;
    }
private:
    REAL m_offset;
    std::default_random_engine m_generator;
    std::uniform_real_distribution<REAL> m_distribution;

public:
    static Random01& singleton()
    {
        static Random01 random;
        return random;
    }
};

vec3 getRandomPointOnUnitSphere( Random01& rand )
{
    while(true)
    {
        vec3 p_in_cube {rand.getRandom(),rand.getRandom(), rand.getRandom()};
        vec3 p_to_center = p_in_cube - vec3(0.5,0.5,0.5);
        if( p_to_center.square_length() > 0.1 )
        {
            return p_to_center.getNormal();
        }
    }
}


#endif //RAYTRACING_RAND_H
