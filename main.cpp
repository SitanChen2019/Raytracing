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
            REAL u = R(c)/width;
            REAL v = R(r)/height;

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
    const REAL camera_w = R(0.16);
    const REAL camera_h = R(0.09);
    const REAL camera_z = R(0.1);

    Camera camera( camera_w, camera_h , camera_z);
    const REAL scope = R(1000);

    int width = int( camera_w*scope);
    int height = int( camera_h*scope);
    Image image(width, height);

    drawBackgorund( image);


    camera.setCameraPose(vec3(0,3,0), V3_UNIT_X, V3_UNIT_Y);

    HitableList world;
    Sphere sphere1(3, vec3(0,3, 10));
    world.addHitable(&sphere1);

    Sphere earth ( 100, vec3(3, -100, 10) );
    world.addHitable( &earth );

    Random01 random(REAL(-0.5));
    for( int r = 0 ; r < height ; ++r ) {
        for (int c = 0; c < width; c++) {
            REAL rW = R(width);
            REAL rH = R(height);

            //multiple samples
            const int N = 8;
            int hitCount = 0;
            vec3 v3SourceColor= vec3(0);
            for( int i = 0; i < N; ++i )
            {
                REAL rC = R(c) + random.getRandom();
                REAL rR = R(r) + random.getRandom();

                Ray ray = camera.makeRay( rC, rR, rW, rH);
                HitInfo hitInfo;
                if( world.rayHit(ray,0, std::numeric_limits<REAL>::max(), hitInfo))
                {
                    hitCount+=1;
                    ColorPixel color= normalToColor(hitInfo.normal);
                    vec3 v3Color = static_cast<vec3>(color);
                    v3SourceColor = v3SourceColor + v3Color ;
                }
            }

            if( hitCount > 0 )
            {
                v3SourceColor = v3SourceColor*inverse(hitCount);
                REAL t= hitCount*inverse(N);
                vec3 targetColor = static_cast<vec3>(image.getPixel(r,c));
                image.setPixel(r,c, lerp(v3SourceColor, targetColor, t) );
            }
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