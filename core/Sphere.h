//
// Created by sitan on 19-8-30.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "Math.h"
#include "IHitable.h"

class Sphere {
public:
    Sphere( REAL r,  vec3 center = {0,0,0})
    {
        r = mR;
        mCenter = center;
    }

    bool rayHit( const Ray& ray, const REAL t_min, const REAL t_max,  HitInfo& hitInfo)
    {
        vec3 v_CO = ray.getOriginal() - mCenter;
        vec3 r_D = ray.getDirection();

        //a*t^2 + b*t + c = 0;
        REAL a = r_D.square_length()
        REAL b = 2*r_D.dot(v_CO);
        REAL c = v_CO.square_length() - mR*mR;

        if( a == 0 )
            //the ray should have a valid direction.
            return false;

        REAL tmp = b*b-4*a*c;
        REAL ax2 = a*2;
        if( isLess(tmp, 0) )
        {
            //no root
            return false;
        }
        else if( isEqual(tmp, 0)  )
        {
            //one root
            REAL root = -b/ax2;
            if( t_min <= root && root <= t_max  )
            {
                makeHitInfo(ray, root, hitInfo);
                return true;
            }
        }
        else
        {
            tmp = std::sqrt(tmp);
            REAL root = (-b-tmp)/ax2;
            if( t_min <= root && root <= t_max  )
            {
                makeHitInfo(ray, root, hitInfo);
                return true;
            }

            //else
            root = (-b + tmp)/ax2;
            if( t_min <= root && root <= t_max  )
            {
                makeHitInfo(ray, root, hitInfo);
                return true;
            }
        }

        return false;
    }

private:
    void makeHitInfo( const Ray& ray, const REAL t ,  HitInfo& hitInfo)
    {
        hitInfo.t = t;
        hitInfo.pos = ray.point_at(t);
        hitInfo.primType = PT_SURFACE;
        hitInfo.normal = (hitInfo.pos - mCenter).getNormal();
    }


private:
    REAL mR;
    vec3 mCenter;

};


#endif //RAYTRACING_SPHERE_H
