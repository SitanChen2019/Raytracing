//
// Created by sitan on 19-9-2.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "Math.h"
#include "Ray.h"
#include "Rand.h"

class Camera
{
private:
    vec3 m_position;

    vec3 m_forwardDirection;

    vec3 m_vDirection;
    vec3 m_uDirection;

    vec3 m_frustum;

    float m_focalratio;  //distance to focal plane / distance to film plane ,
    float m_aperture;   //lens size

public:
    Camera(const REAL width, const REAL height, const REAL depth)
    {
        m_frustum = vec3( width, height, depth);
        m_aperture = depth / 2;
        m_focalratio = 1;
    }

    void setCameraPose( const vec3& pos, const vec3& udir, const vec3& vdir)
    {
        m_position = pos;
        m_uDirection = udir.getNormal();
        m_vDirection = vdir.getNormal();

        m_forwardDirection = m_uDirection.product(m_vDirection);
    }

    void lookAt( const vec3& eye, const vec3 target, const vec3 up)
    {
        m_position = eye;
        m_forwardDirection = (target - eye).getNormal();

        m_uDirection = up.product( m_forwardDirection  );
        m_vDirection = m_forwardDirection.product( m_uDirection );
    }


    void setFocalRatio( float value )
    {
        m_focalratio = value;
    }

    void focusToPoint( vec3 point )
    {
        float distance = (point - m_position).length();
        m_focalratio = (distance - m_frustum.z)/m_frustum.z;
    }

    void setAperture( float value )
    {
        m_aperture = value;
    }

    Ray makeRay( REAL x, REAL y, REAL width, REAL height  )
    {
        vec3 apetureRand = m_aperture * 0.5f * getRandomPointOnUnitDisk( Random01::singleton() );
        vec3 origin = m_position  + apetureRand.x * m_uDirection + apetureRand.y * m_vDirection ;
        vec3 target = m_position +  (x - width/2)/width*m_frustum.x*m_uDirection * m_focalratio
                + (y - height/2)/height*(-m_frustum.y)*m_vDirection * m_focalratio
                + m_frustum.z*m_forwardDirection * (1+m_focalratio);

        return Ray( origin, target - origin);
    }

};
#endif //RAYTRACING_CAMERA_H
