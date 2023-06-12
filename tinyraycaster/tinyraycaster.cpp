#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <sstream>
#include <iomanip>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


const uint32_t red = 255;
const uint32_t green = 255<<8;
const uint32_t blue = 255<<16;
const uint32_t white = red+green+blue;

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a=255){
    return (a<<24) +(b<<16) + (g<<8) + r;
}

void unpack_color(const uint32_t& color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a){
    r = (color>>0) & 255;
    g = (color>>8) & 255;
    b = (color>>16) & 255;
    a = (color>>24) & 255;
}

void save_ppm_image(const std::string filename, const std::vector<uint32_t> &image, const uint32_t w, const uint32_t h){
    assert(image.size()==w*h);
    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P6\n" << w << " " << h << "\n255\n";
    for(size_t i = 0; i < w*h; i++){
        uint8_t r, g, b, a;
        unpack_color(image[i], r, g, b, a);
        ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
    }
    ofs.close();
}

void create_map(std::vector<uint32_t> &image, const size_t w, const size_t h, const char* map, const uint32_t mapw, const uint32_t maph){
    
    uint32_t dw = 0.5*w/mapw; 
    uint32_t dh = h/maph; 
    for(size_t j=0;j<h;j++){
        for(size_t i=0;i<w/2;i++){
            //if(map[(int)(i/dw) + (int)(j/dh)*maph] != ' ') continue;
            if(map[(int)(i/dw) + (int)(j/dh)*maph] == '0') image[j*w+i] = pack_color(168, 250, 23);
            else if(map[(int)(i/dw) + (int)(j/dh)*maph] == '1') image[j*w+i] = pack_color(90, 156, 200);
            else if(map[(int)(i/dw) + (int)(j/dh)*maph] == '2') image[j*w+i] = pack_color(45, 250, 11);
            else if(map[(int)(i/dw) + (int)(j/dh)*maph] == '3') image[j*w+i] = pack_color(255, 16, 160);
            // uint8_t r = 255*i/float(w);
            // uint8_t g = 255*j/float(h);
            // uint8_t b = 0;

            else image[j*w+i] = white;
        }
    }   
}


void create_player(std::vector<uint32_t> &image, const size_t w, const size_t h, float nowx, float nowy, float view, const char* map, const uint32_t mapw, const uint32_t maph, float temp, float fov){
    float incre = 0.2;
    size_t count=0;
    uint32_t dw = 0.5*w/mapw; 
    uint32_t dh = h/maph;
    while(map[(int)(nowx/dw) + (int)(nowy/dh)*maph] == ' '){
        image[(int)nowx + (int)nowy*w] = 0;
        nowx += incre*cos(view);
        nowy += incre*sin(view);
        count++;
    }
    uint32_t color = pack_color(255, 255, 255);
    if(map[(int)(nowx/dw) + (int)(nowy/dh)*maph] == '0') color = pack_color(168, 250, 23);
    else if(map[(int)(nowx/dw) + (int)(nowy/dh)*maph] == '1') color = pack_color(90, 156, 200);
    else if(map[(int)(nowx/dw) + (int)(nowy/dh)*maph] == '2') color = pack_color(45, 250, 11);
    else if(map[(int)(nowx/dw) + (int)(nowy/dh)*maph] == '3') color = pack_color(255, 16, 160);
    uint32_t colum_lens = 250*h/count/cos(temp- fov/2);
    for(size_t j=0;j<colum_lens;j++){
        if(w/2+(int)(temp/(2/(float)w))<w)
        image[w/2+(int)(temp/(2/(float)w))+ (h/2 - colum_lens/2 + j)*w] = color;
    }

}

int main(){
    uint32_t backcolor = green + blue;
    const size_t width = 1024;
    const size_t height = 512;
    std::vector<uint32_t> framebuffer(width*height, backcolor);
    float player_posx = 100;
    float player_posy = 90;
    float view = 1.33;
    float fov = M_PI/3.; 
    const uint32_t mapw = 16;
    const uint32_t maph = 16;
    const char map[] = "0000222222220000"\
                        "1              0"\
                        "1      11111   0"\
                        "1     0        0"\
                        "0     0  1110000"\
                        "0     3        0"\
                        "0   10000      0"\
                        "0   0   11100  0"\
                        "0   0   0      0"\
                        "0   0   1  00000"\
                        "0       1      0"\
                        "2       1      0"\
                        "0       0      0"\
                        "0 0000000      0"\
                        "0              0"\
                        "0002222222200000";
    for (size_t frame=0; frame<360; frame++) {
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        view += 2*M_PI/360;
        create_map(framebuffer, width, height, map, mapw, maph);
        for(size_t j=0;j<height;j++){
            for(size_t i =width/2;i<width;i++){
                framebuffer[i+j*width] = backcolor;
            }
        }
        for(float temp = 0.; temp<fov ;temp+=2/(float)width)
            create_player(framebuffer, width, height, player_posx, player_posy, view-fov/2+temp, map, mapw, maph, temp, fov);
        
        save_ppm_image(ss.str(), framebuffer, width, height);
    }
    // save_ppm_image("output.ppm", framebuffer, width, height);
    return 0;    
}