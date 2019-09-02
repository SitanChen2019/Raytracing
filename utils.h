//
// Created by sitan on 19-9-2.
//

#ifndef RAYTRACING_UTILS_H
#define RAYTRACING_UTILS_H

#include "core/core.h"

void drawBackgorund( Image& image)
{

    int width = image.getWidth();
    int height = image.getHeight();
    for( int r = 0 ; r < height ; ++r )
    {
        for(int c = 0 ; c < width ; c++ )
        {
            REAL u = REAL(c)/width;
            REAL v = REAL(r)/height;

            vec3 color1 = vec3(1,1,1);
            vec3 color2 = vec3(0.5,0.7,1.0);
            vec3 color = lerp(color1, color2, v);


            ColorPixel pixel = color;
            image.setPixel(r,c, pixel);
        }
    }
}


ColorPixel normalToColor( vec3 normalDir )
{
    std::cout <<normalDir << std::endl;
    //assume normalDir already normalized
    vec3 tmp = (normalDir+vec3(1))*REAL(0.5);
    tmp.g = 0;
    tmp.b = 0;
    return ColorPixel(tmp);
}

#endif //RAYTRACING_UTILS_H
