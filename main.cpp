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


vec3 bgColorFromRay( const Ray& ray )
{
    float t = ray.getDirection().y * 0.5f + 0.5f;
    return lerp( vec3(0.5,0.7,1.0), vec3(1,1,1),t);
}

vec3 colorFromRay(  const Ray& ray, IHitable* world , int depth = 0)
{
    if( depth > 50)
        return vec3(0);
    HitInfo hitInfo;
    const REAL  t_min = 1e-3;
    const REAL t_max = std::numeric_limits<REAL>::max();
    if( world->rayHit(ray, t_min, t_max, hitInfo ) )
    {
        auto pair = hitInfo.pMaterial->getScatterRay(ray,hitInfo);
        auto ret = pair.first * colorFromRay( pair.second, world, depth + 1);
        return ret;
    }
    return bgColorFromRay(ray);
}

void chapter7()
{
    const REAL camera_w = R(0.16);
    const REAL camera_h = R(0.09);
    const REAL camera_z = R(0.05);

    Camera camera( camera_w, camera_h , camera_z);
    const REAL scope = R(3000);

    int width = int( camera_w*scope);
    int height = int( camera_h*scope);
    Image image(width, height);

    drawBackgorund( image);


    camera.setCameraPose(vec3(0,0,0), -V3_UNIT_X, V3_UNIT_Y);

    HitableList world;
    vec3 material = vec3(0.5,0.5,0.5);

    Sphere sphere1(0.5, vec3(0,0, -1));
    world.addHitable(&sphere1);
    Material& mat1 = sphere1.getMaterial();
    mat1.setAlebdo(vec3(0.8,0.3,0.3));
    mat1.setScatterMode(RANDOM_DIFFUSE);

    ScatterModeData scatterData;
    scatterData.disturb_radio = 0.1f;
    Sphere sphere2(0.5, vec3(1,0, -1));
    world.addHitable(&sphere2);
    Material& mat2 = sphere2.getMaterial();
    mat2.setAlebdo(vec3(0.5,0.9,0.5));
    mat2.setScatterMode(MIRROR_DIFFUSE_WITH_DISTURB,scatterData);

    Sphere sphere3(0.5, vec3(-1,0, -1));
    world.addHitable(&sphere3);
    Material& mat3 = sphere3.getMaterial();
    mat3.setAlebdo(vec3(0.8,0.8,0.8));
    scatterData.disturb_radio = 1.0f;
    mat3.setScatterMode(MIRROR_DIFFUSE_WITH_DISTURB,scatterData);

    Sphere earth ( 100, vec3(0, -100.5, -1) );
    earth.getMaterial().setAlebdo(vec3(0.8,0.8,0));
    earth.getMaterial().setScatterMode( RANDOM_DIFFUSE );
    world.addHitable( &earth );

    Random01 random(REAL(-0.5));
    for( int r = 0 ; r < height ; ++r ) {
        for (int c = 0; c < width; c++) {
            REAL rW = R(width);
            REAL rH = R(height);

            //multiple samples
            const int N = 50;
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
                    vec3 v3Color = colorFromRay(ray, &world);
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


void chapter9()
{
    const REAL camera_w = R(0.16);
    const REAL camera_h = R(0.09);
    const REAL camera_z = R(0.05);

    Camera camera( camera_w, camera_h , camera_z);
    const REAL scope = R(3000);

    int width = int( camera_w*scope);
    int height = int( camera_h*scope);
    Image image(width, height);

    drawBackgorund( image);


    camera.setCameraPose(vec3(0,0,0), -V3_UNIT_X, V3_UNIT_Y);

    HitableList world;
    vec3 material = vec3(0.5,0.5,0.5);

    Sphere sphere1(0.25, vec3(0,0, -1));
    world.addHitable(&sphere1);
    Material& mat1 = sphere1.getMaterial();
    mat1.setAlebdo(vec3(0.8,0.3,0.3));
    mat1.setScatterMode(RANDOM_DIFFUSE);
    mat1.setRefraction(true,1.53f);

    Sphere sphere2(0.5, vec3(0.5,0, -1.5));
    world.addHitable(&sphere2);
    Material& mat2 = sphere2.getMaterial();
    mat2.setAlebdo(vec3(0.3,0.8,0.3));
    mat2.setScatterMode(RANDOM_DIFFUSE);

    Sphere sphere3(0.5, vec3(-0.5,0, -1.5));
    world.addHitable(&sphere3);
    Material& mat3 = sphere3.getMaterial();
    mat3.setAlebdo(vec3(0.3,0.3,0.8));
    mat3.setScatterMode(MIRROR_DIFFUSE);

    Sphere earth ( 100, vec3(0, -100.5, -1) );
    earth.getMaterial().setAlebdo(vec3(0.8,0.8,0));
    earth.getMaterial().setScatterMode( RANDOM_DIFFUSE );
    world.addHitable( &earth );

    Random01 random(REAL(-0.5));
    for( int r = 0 ; r < height ; ++r ) {
        for (int c = 0; c < width; c++) {
            REAL rW = R(width);
            REAL rH = R(height);

            //multiple samples
            const int N = 50;

            vec3 v3SourceColor= vec3(0);
            for( int i = 0; i < N; ++i )
            {
                REAL rC = R(c) + random.getRandom();
                REAL rR = R(r) + random.getRandom();

                Ray ray = camera.makeRay( rC, rR, rW, rH);
                HitInfo hitInfo;
                if( world.rayHit(ray,0, std::numeric_limits<REAL>::max(), hitInfo))
                {
                    vec3 v3Color = colorFromRay(ray, &world);
                    v3SourceColor = v3SourceColor + v3Color ;
                }
            }

            {
                image.setPixel(r,c, v3SourceColor*inverse<float>(N) );
            }
        }
    }

    image.savePPM("cst.ppm");
}

void chapter11()
{
    const REAL camera_w = R(0.16);
    const REAL camera_h = R(0.09);
    const REAL camera_z = R(0.15);

    Camera camera( camera_w, camera_h , camera_z);
    const REAL scope = R(3000);

    int width = int( camera_w*scope);
    int height = int( camera_h*scope);
    Image image(width, height);

    vec3 eye = vec3(0,0,0);
    eye = vec3(2,3,1);
    camera.lookAt(eye, vec3(0,0,-1), vec3(0,1,0));
    camera.setAperture( 0.01 );

    HitableList world;
    vec3 material = vec3(0.5,0.5,0.5);

    Sphere sphere1(0.5, vec3(0,0, -1));
    world.addHitable(&sphere1);
    Material& mat1 = sphere1.getMaterial();
    mat1.setAlebdo(vec3(0.1,0.2,0.5));
    mat1.setScatterMode(RANDOM_DIFFUSE);
    mat1.setRefraction(false,1.53f);

    Sphere sphere2(0.5, vec3(1,0, -1));
    world.addHitable(&sphere2);
    Material& mat2 = sphere2.getMaterial();
    mat2.setAlebdo(vec3(0.8,0.6,0.2));
    ScatterModeData scatterData;
    scatterData.disturb_radio = 1.0;
    mat2.setScatterMode(MIRROR_DIFFUSE_WITH_DISTURB, scatterData);

    Sphere sphere3(0.5, vec3(-1,0, -1));
    world.addHitable(&sphere3);
    Material& mat3 = sphere3.getMaterial();
    mat3.setAlebdo(vec3(1,1,1));
    mat3.setScatterMode(MIRROR_DIFFUSE);
    mat3.setRefraction(true, 1.5);

    Sphere sphere4(0.45, vec3(-1,0, -1));
    world.addHitable(&sphere4);
    Material& mat4 = sphere4.getMaterial();
    mat4.setAlebdo(vec3(1,1,1));
    mat4.setScatterMode(MIRROR_DIFFUSE);

    Sphere earth ( 100, vec3(0, -100.5, -1) );
    earth.getMaterial().setAlebdo(vec3(0.8,0.8,0));
    earth.getMaterial().setScatterMode( RANDOM_DIFFUSE );
    world.addHitable( &earth );

    camera.focusToPoint( sphere1.center() );

    Random01 random(REAL(-0.5));
    for( int r = 0 ; r < height ; ++r ) {
        for (int c = 0; c < width; c++) {
            REAL rW = R(width);
            REAL rH = R(height);

            //multiple samples
            const int N = 50;
            vec3 v3SourceColor= vec3(0);
            for( int i = 0; i < N; ++i )
            {
                REAL rC = R(c) + random.getRandom();
                REAL rR = R(r) + random.getRandom();

                Ray ray = camera.makeRay( rC, rR, rW, rH);
                HitInfo hitInfo;
                //if( world.rayHit(ray,0, std::numeric_limits<REAL>::max(), hitInfo))
                {

                    vec3 v3Color = colorFromRay(ray, &world);
                    if( v3Color.length() < 0.01 )
                    {
                        v3Color = colorFromRay( ray, &world);
                    }
                    v3SourceColor = v3SourceColor + v3Color ;
                }
            }

            //if( hitCount > 0 )
            {
                image.setPixel(r,c, v3SourceColor*inverse(N) );
            }
        }
    }

    image.savePPM("cst.ppm");
}

void chapter12()
{
    const REAL camera_w = R(0.16);
    const REAL camera_h = R(0.09);
    const REAL camera_z = R(0.2);

    Camera camera( camera_w, camera_h , camera_z);
    const REAL scope = R(3000);

    int width = int( camera_w*scope);
    int height = int( camera_h*scope);
    Image image(width, height);


    vec3 eye = vec3(0,0,0);
    eye = vec3(13,2,3);
    camera.lookAt(eye, vec3(0,0,-1), vec3(0,1,0));
    camera.setAperture( 0 );


    //build the world
    HitableList world;
    Sphere earth = Sphere( 1000, vec3(0,-1000,0));
    earth.getMaterial().setAlebdo( vec3(0.5,0.5,0.5));
    earth.getMaterial().setScatterMode(RANDOM_DIFFUSE);
    world.addHitable( &earth);

    Sphere ball1 = Sphere( 1, vec3( -4,1,0));
    ball1.getMaterial().setAlebdo(vec3(0.4,0.2,0.1));
    ball1.getMaterial().setScatterMode( RANDOM_DIFFUSE );
    world.addHitable( &ball1 );

    Sphere ball2 = Sphere( 1, vec3(4,1,0));
    ball2.getMaterial().setAlebdo(vec3(0.7,0.6,0.5));
    ball2.getMaterial().setScatterMode(MIRROR_DIFFUSE);
    world.addHitable(&ball2);

    Sphere ball3 = Sphere(1,vec3(0,1,0));
    ball3.getMaterial().setAlebdo(vec3(1,1,1));
    ball3.getMaterial().setScatterMode(MIRROR_DIFFUSE);
    ball3.getMaterial().setRefraction(true,1.5f);
    world.addHitable(&ball3);

    Random01 random(REAL(-0.5));

    std::vector<Sphere> smallBalls;
    for( int r = -11; r < 11; ++r )
    {
        for( int c = -11; c < 11 ; ++c )
        {
            vec3 center {float(r) + 0.6f*random.getRandom(),0.2f, float(c) + 0.6f*random.getRandom() };
            smallBalls.push_back({0.2,center});
            Material& material = smallBalls.back().getMaterial();

            float mat_choose=  random.getRandom();
            if( mat_choose < 0.8)
            {
                //diffuse
                material.setAlebdo( vec3( random.getRandom()*random.getRandom(),
                        random.getRandom()*random.getRandom(),
                        random.getRandom()*random.getRandom() ) );
            }
            else if( mat_choose < 0.9 )
            {
                //mirror
                material.setAlebdo( 0.5f*vec3( 1+random.getRandom(),1+random.getRandom(),1+random.getRandom() )
                                           );
                ScatterModeData data;
                data.disturb_radio = random.getRandom()*0.5f;
                material.setScatterMode( MIRROR_DIFFUSE_WITH_DISTURB, data);
            } else
            {
                material.setAlebdo(vec3(1,1,1));
                material.setRefraction( true, 1.5f);
            }
        }
    }

    for( auto& ball : smallBalls )
        world.addHitable(&ball);
    camera.focusToPoint( ball2.center() );




    for( int r = 0 ; r < height ; ++r ) {
        std::cout << "Percentage: " << int( r*100.0f/height ) << "%" << std::endl;
        for (int c = 0; c < width; c++) {
            REAL rW = R(width);
            REAL rH = R(height);

            //multiple samples
            const int N = 100;
            vec3 v3SourceColor= vec3(0);
            for( int i = 0; i < N; ++i )
            {
                REAL rC = R(c) + random.getRandom();
                REAL rR = R(r) + random.getRandom();

                Ray ray = camera.makeRay( rC, rR, rW, rH);
                HitInfo hitInfo;
                //if( world.rayHit(ray,0, std::numeric_limits<REAL>::max(), hitInfo))
                {

                    vec3 v3Color = colorFromRay(ray, &world);
                    if( v3Color.length() < 0.01 )
                    {
                        v3Color = colorFromRay( ray, &world);
                    }
                    v3SourceColor = v3SourceColor + v3Color ;
                }
            }

            //if( hitCount > 0 )
            {
                image.setPixel(r,c, v3SourceColor*inverse(N) );
            }
        }
    }

    image.savePPM("cst.ppm");
};


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
    //chapter3();
    //chapter7();
    //chapter9();
    //chapter11();
    chapter12();
    return 0;
}