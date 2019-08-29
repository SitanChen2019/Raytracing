#include <iostream>
#include "core/Image.h"


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

}

int main()
{
    chapter1();
    return 0;
}