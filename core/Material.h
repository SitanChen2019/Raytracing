//
// Created by Sitan Chen on 2019/9/6.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H

#include <exception>
#include "Ray.h"
#include "IHitable.h"
#include "Rand.h"
#include <memory>

enum ScatterMode
{
    RANDOM_DIFFUSE,
    MIRROR_DIFFUSE,
    MIRROR_DIFFUSE_WITH_DISTURB
};


struct ScatterModeData
{
    float disturb_radio = 0.1f;
};
bool isTotalRefract( const Ray inputRay, const HitInfo& hitInfo );
Ray InverseRefractRay( const Ray inputRay, const HitInfo& hitInfo  );
float schlick(const Ray inputRay, const HitInfo& hitInfo );

Ray InverseReflectRay(const Ray inputRay, const HitInfo&  hitInfo, ScatterMode mode, ScatterModeData data = ScatterModeData{})
{
    if( mode == RANDOM_DIFFUSE)
    {
        vec3 target = hitInfo.pos + hitInfo.normal + getRandomPointOnUnitSphere( Random01::singleton());
        return Ray( hitInfo.pos, target - hitInfo.pos);
    }
    else if( mode == MIRROR_DIFFUSE)
    {
        vec3 outDir = inputRay.getDirection() - 2*inputRay.getDirection().dot( hitInfo.normal )*hitInfo.normal;
        return Ray(hitInfo.pos, outDir);
    }
    else if( mode == MIRROR_DIFFUSE_WITH_DISTURB )
    {
        vec3 outDir = inputRay.getDirection() - 2*inputRay.getDirection().dot( hitInfo.normal )*hitInfo.normal;
        vec3 target = hitInfo.pos + outDir.getNormal() + data.disturb_radio*getRandomPointOnUnitSphere( Random01::singleton());
        return Ray( hitInfo.pos, target - hitInfo.pos);
    }

    else
    {
        throw std::exception();
    }
    return inputRay;
}




class Material {
private:
    ScatterMode  mScatterMode = MIRROR_DIFFUSE;
    ScatterModeData mScatterData;
    vec3 mAlbedo = vec3(0.5f);
    bool mRefractAble =  false ;
    float mRefractiveIndice = 1; //air : 1 wather 1.33 Grown glass 1.52 Flint glass 1.66


public:
    std::pair<vec3, Ray> getScatterRay( const Ray& ray , const HitInfo& hitInfo)
    {
        float reflectProbability = 1;

        if( mRefractAble )
        {
            float reflectProbability = schlick( ray, hitInfo);
            reflectProbability = 0;

            if (isTotalRefract(ray, hitInfo))
                reflectProbability = 1;
        }

        float randomNumber = Random01::singleton().getRandom();

        if( randomNumber <= reflectProbability )
        {
            return std::make_pair( mAlbedo, InverseReflectRay(ray, hitInfo, mScatterMode));
        }
        else
        {
            return std::make_pair( vec3(1),  InverseRefractRay( ray, hitInfo ));
        }
    }

    vec3 getAlgedo()
    {
        return mAlbedo;
    }

    void setAlebdo(vec3 albedo )
    {
        mAlbedo = albedo;
    }

    float getRefactorIndices()
    {
        return mRefractiveIndice;
    }

    void setScatterMode(ScatterMode mode,ScatterModeData scatterData = ScatterModeData{})
    {
        mScatterMode =  mode;
        mScatterData = scatterData;
    }

    void setRefraction(  bool bRefractable, float indices )
    {
        mRefractAble = bRefractable;
        if( bRefractable )
        {
            assert( indices > 0);
            mRefractiveIndice = indices;
        }
    }

};


Ray InverseRefractRay( const Ray inputRay, const HitInfo& hitInfo  )
{
    float inputIndice = 1.f;
    float outIndices = 1.f;

    float flag = 1.f;
    if( inputRay.getDirection().dot( hitInfo.normal) >= 0 )
    {
        inputIndice = hitInfo.pMaterial->getRefactorIndices();
    } else{
        flag = -1.f;
        outIndices =  hitInfo.pMaterial->getRefactorIndices();
    }

    auto tmpNormal = flag*hitInfo.normal;
    auto inputDir = inputRay.getDirection();
    auto tangentDir = inputDir - inputDir.dot( tmpNormal ) * tmpNormal;
    tangentDir.normalize();

    auto sin_a1 = inputDir.product(tmpNormal).length();
    auto sin_a2 =sin_a1*inputIndice/outIndices;

    assert( sin_a2 <= 1);
    auto cos_a2 = sqrt(1 - sin_a1*sin_a1);

    auto reflect_dir = sin_a2*tangentDir + cos_a2*tmpNormal;

    return  Ray( hitInfo.pos, reflect_dir);;
}

bool isTotalRefract( const Ray inputRay, const HitInfo& hitInfo )
{
    float inputIndice = 1.f;
    float outIndices = 1.f;

    float flag = 1.f;
    if( inputRay.getDirection().dot( hitInfo.normal) >= 0 )
    {
        inputIndice = hitInfo.pMaterial->getRefactorIndices();
    } else{
        flag = -1.f;
        outIndices =  hitInfo.pMaterial->getRefactorIndices();
    }

    if( inputIndice <= outIndices )
        return false;

    float sin_input = inputRay.getDirection().product(flag*hitInfo.normal).length();

    if( inputIndice*sin_input/outIndices >1 )
        return true;
    else
        return false;
}

float schlick(const Ray inputRay, const HitInfo& hitInfo )
{
    //Christophe Schlick approximation
    //To detemine how much light is reflected and how many are refracted

    float inputIndice = 1.f;
    float outIndices = 1.f;

    float flag = 1.f;
    if( inputRay.getDirection().dot( hitInfo.normal) >= 0 )
    {
        inputIndice = hitInfo.pMaterial->getRefactorIndices();
    } else{
        flag = -1.f;
        outIndices =  hitInfo.pMaterial->getRefactorIndices();
    }

    auto tmpNormal = flag*hitInfo.normal;
    auto inputDir = inputRay.getDirection();
    auto tangentDir = inputDir - inputDir.dot( tmpNormal ) * tmpNormal;
    tangentDir.normalize();

    auto sin_a1 = inputDir.product(tmpNormal).length();
    auto cos_a1 = sqrt( 1 - sin_a1*sin_a1);

    float  R0 = pow(  (inputIndice - outIndices) / (inputIndice + outIndices ) ,2 );
    return R0 + (1 - R0)* pow( 1-cos_a1, 5);

}

#endif //RAYTRACING_MATERIAL_H


