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
bool test();


Ray InverseReflectRay(const Ray inputRay, const HitInfo&  hitInfo, ScatterMode mode, ScatterModeData data = ScatterModeData{})
{
    vec3 normal = hitInfo.normal;

    if( normal.dot( inputRay.getDirection() ) > 0 )
        normal = - normal;


    if( mode == RANDOM_DIFFUSE)
    {
        vec3 target = hitInfo.pos + normal + getRandomPointOnUnitSphere( Random01::singleton());
        return Ray( hitInfo.pos, target - hitInfo.pos);
    }
    else if( mode == MIRROR_DIFFUSE)
    {
        vec3 outDir = inputRay.getDirection() - 2*inputRay.getDirection().dot( normal )*normal;
        return Ray(hitInfo.pos, outDir);
    }
    else if( mode == MIRROR_DIFFUSE_WITH_DISTURB )
    {
        vec3 outDir = inputRay.getDirection() - 2*inputRay.getDirection().dot( normal )*normal;
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
        //test();
        float reflectProbability = 1;

        if( mRefractAble )
        {
            reflectProbability = schlick( ray, hitInfo);
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


bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = v.getNormal();
    float dt = uv.dot(n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}


Ray InverseRefractRay( const Ray inputRay, const HitInfo& hitInfo  )
{
    float inputIndice = 1.f;
    float outIndices = 1.f;

    float flag = 1.0f;
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
    auto cos_a2 = sqrt(1 - sin_a2*sin_a2);

    assert(tangentDir.dot(tmpNormal) < 0.01);
    auto reflect_dir = sin_a2*tangentDir + cos_a2*tmpNormal;
    reflect_dir.normalize();

    //vec3 reflect_dir2(-reflect_dir);
    //refract( inputRay.getDirection(), tmpNormal, inputIndice/outIndices, reflect_dir2);
    //assert( reflect_dir.dot(reflect_dir2) > 0.9);
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

bool test()
{
    HitInfo hitInfo;
    hitInfo.pos = vec3(0);
    hitInfo.normal = vec3(0,1,0);
    hitInfo.pMaterial = new Material();
    hitInfo.pMaterial->setRefraction(true,2);

    Ray inputRay( vec3(-0.5f, sqrt(3.0f)/2.0f, 0 ), vec3(0.5f, -sqrt(3.0f)/2,0));
    auto outRay = InverseRefractRay( inputRay,  hitInfo );
    if( ( outRay.getOriginal() - hitInfo.pos ).length() > 0.001)
        return false;
    vec3 expect_value = vec3(0.25f, -sqrt(15.0f)/4.0f, 0 );
    if( outRay.getDirection().dot( expect_value ) < 0.99 )
        return false;

    return true;
}

#endif //RAYTRACING_MATERIAL_H


