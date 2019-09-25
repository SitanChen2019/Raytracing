//
// Created by sitan on 19-8-30.
//

#ifndef RAYTRACING_IHITABLE_H
#define RAYTRACING_IHITABLE_H

#include "Math.h"
#include "Ray.h"

class Material;

enum PrimitiveType
{
    PT_POINT,
    PT_LINE,
    PT_SURFACE
};

struct HitInfo
{
    REAL t;
    vec3 pos;
    vec3 normal;
    PrimitiveType  primType;
    Material* pMaterial = nullptr;
    bool isInRaySide = false;
};

class IHitable
{
public:
    virtual bool rayHit( const Ray& ray, const REAL t_min, const REAL t_max,  HitInfo& hitInfo) = 0;
};


#endif //RAYTRACING_IHITABLE_H
