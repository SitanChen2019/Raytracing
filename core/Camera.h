//
// Created by sitan on 19-9-2.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "Math.h"
#include "Ray.h"

class Camera
{
private:
    vec3 m_position;

    vec3 m_forwardDirection;

    vec3 m_vDirection;
    vec3 m_uDirection;

    vec3 m_frustum;

public:
    Camera(const REAL width, const REAL height, const REAL depth)
    {
        m_frustum = vec3( width, height, depth);
    }

    void setCameraPose( const vec3& pos, const vec3& udir, const vec3& vdir)
    {
        m_position = pos;
        m_uDirection = udir.getNormal();
        m_vDirection = vdir.getNormal();

        m_forwardDirection = m_uDirection.product(m_vDirection);
    }



    Ray makeRay( REAL x, REAL y, REAL width, REAL height  )
    {
        vec3 origin = m_position;
        vec3 target = m_position + (x - width/2)/width*m_frustum.x*m_uDirection
                + (y - height/2)/height*(-m_frustum.y)*m_vDirection
                + m_frustum.z*m_forwardDirection;
        return Ray( origin, target - origin);
    }

};
#endif //RAYTRACING_CAMERA_H
