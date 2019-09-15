//
// Created by Sitan Chen on 2019/9/6.
//

#ifndef RAYTRACING_IBODY_H
#define RAYTRACING_IBODY_H

class Material;

class IBody  : public  IHitable {
protected:
    ~IBody()
    {}

    Material mMaterial;

public:
    Material& getMaterial(  )
    {
        return mMaterial;
    }
};

#endif //RAYTRACING_IBODY_H
