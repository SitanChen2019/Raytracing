//
// Created by sitan on 19-9-2.
//

#ifndef RAYTRACING_HITABLELIST_H
#define RAYTRACING_HITABLELIST_H

#include "IHitable.h"
#include <set>

class HitableList : public  IHitable {

public:
    void addHitable( IHitable* pHitable )
    {
        m_HitableList.insert(pHitable);
    }

    void removeHitable( IHitable* pHitable)
    {
        m_HitableList.erase(pHitable);
    }

    bool rayHit( const Ray& ray, const REAL t_min, const REAL t_max,  HitInfo& hitInfo) override
    {
        REAL currentMax = t_max;

        bool bHit = false;
        for( auto hitable : m_HitableList )
        {
            HitInfo currentInfo;
            if( hitable->rayHit(ray, t_min, currentMax, currentInfo) )
            {
                bHit = true;
                if( currentInfo.t < currentMax )
                    currentMax = currentInfo.t;
                hitInfo = currentInfo;
            }
        }

        return bHit;
    }


private:
    std::set<IHitable*> m_HitableList;
};

#endif //RAYTRACING_HITABLELIST_H
