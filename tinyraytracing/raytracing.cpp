#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include "utils.h"
#include "geometry.h"

const uint32_t red = 255;
const uint32_t green = 255<<8;
const uint32_t blue = 255<<16;
const uint32_t white = red + blue + green;



int main(){
    const uint32_t width = 512;
    const uint32_t height = 512;
    std::vector<uint32_t> framebuffer(width*height, 0);

    for(size_t j=0;j<height;j++){
        for(size_t i=0;i<width;i++){
            uint8_t r = 255*i/(float)width;
            uint8_t g = 255*j/(float)height;
            framebuffer[i+j*width] = pack_color(r, g, 0);
        }
    }
    

    save_ppm("out.ppm", framebuffer, width, height);
    return 0;
}