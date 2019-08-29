//
// Created by sitan on 19-8-29.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "Math.h"

class Ray {
public:
    Ray( vec3 pos = vec3{0,0,0}, vec3 dir = vec3{0,0,1})
    {
        mPos = pos;
        mDir = dir;
        mDir.normalize();
    }

    vec3 getOriginal() const {return mPos;} ;
    vec3 getDirection() const { return mDir; } ;

    vec3 point_at(REAL t)
    {
        return mPos+mDir*t;
    }

 private:
    vec3 mPos;
    vec3 mDir;
};


#endif //RAYTRACING_RAY_H
