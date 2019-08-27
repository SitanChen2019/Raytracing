//
// Created by sitan on 19-8-27.
//

#ifndef RAYTRACING_IMAGE_H
#define RAYTRACING_IMAGE_H

#include <fstream>
#include <vector>
#include <assert.h>
#include <string>
#include <iostream>

struct ColorPixel
{
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
    unsigned char a = 255;

    ColorPixel(){};
    ColorPixel(char v)
    {
        r = v;
        g = v;
        b = v;
        a = 255;
    }

    ColorPixel(char _r, char _g, char _b)
    {
        r = _r;
        g = _g;
        b = _b;
        a = 255;
    }

    ColorPixel(char _r, char _g, char _b, char _a)
    {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }
};

class Image
{
public:
    Image( int width , int height )
    :mWidth(width), mHeight(height)
    {
        assert(width >0 && height > 0);
        m_pixels.resize(width*height);
    }

    void savePPM(const char* filename)
    {
        std::ofstream outfile(  filename );
        //write the head
        outfile << "P3" << "\n" << mWidth << " " << mHeight <<"\n" << 255 << "\n";


        for( int r = 0; r < mHeight ; ++r )
        {
            ColorPixel* pPixel = (ColorPixel*)m_pixels.data()+r*mWidth;
            for( int c = 0; c < mWidth; ++c )
            {
                ColorPixel* pCurrentPixel = pPixel + c;
                outfile << unsigned(pCurrentPixel->r)  << " " << unsigned(pCurrentPixel->g) << " " << unsigned(pCurrentPixel->b) << "\n";
            }
        }

        outfile.close();
    }



    bool setPixel( int row, int col, const ColorPixel& pixel )
    {
        m_pixels[row*mWidth+col] = pixel;
    }

private:
    int mWidth;
    int mHeight;
    std::vector<ColorPixel> m_pixels;
};




#endif //RAYTRACING_IMAGE_H
