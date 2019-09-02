#include <iostream>
#include "core/core.h"
#include "utils.h"

void chapter1() {

    int width = 200;
    int height = 100;
    Image image(width, height);

    for( int r = 0 ; r < height ; ++r )
    {
        for(int c = 0 ; c < width ; c++ )
        {
            ColorPixel pixel;
            pixel.r = (char)(255*r*1.0/(height-1) );
            pixel.g = 0;
            pixel.b = 0;

            image.setPixel(r,c, pixel);
        }
    }

    image.savePPM("cst.ppm");
}

void chapter2() {

    int width = 200;
    int height = 100;
    Image image(width, height);

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

    image.savePPM("cst.ppm");
}

void chapter3() {
    int width = 200;
    int height = 100;
    Image image(width, height);

    drawBackgorund( image);

    Camera camera( REAL(0.2), REAL(0.1), REAL(0.05));
    camera.setCameraPose(vec3(0,0,0), V3_UNIT_X, V3_UNIT_Y);

    Sphere sphere(3, vec3(0,0, 10));

    for( int r = 0 ; r < height ; ++r ) {
        for (int c = 0; c < width; c++) {
            Ray ray = camera.makeRay( c, r, width, height);

            image.setPixel(r,c, normalToColor(ray.getDirection()));

//            HitInfo hitInfo;
//            if( sphere.rayHit(ray,0, std::numeric_limits<REAL>::max(), hitInfo))
//            {
//                std::cout << "Hit at " << hitInfo.pos << std::endl;
//            }
        }
    }

    image.savePPM("cst.ppm");
}

void quick_test()
{
    vec3 v(1,1,1);
    std::cout << v*0.1f << std::endl;
    std::cout << 0.2f*v <<std::endl;
}

int main()
{
    //chapter1();
    //chapter2();
    //quick_test();
    chapter3();
    return 0;
}