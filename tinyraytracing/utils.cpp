#include "utils.h"

uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    return r + (g<<8) + (b<<16) + (a<<24);
}

void unpack_color(uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a){
    r = (color>>0) & 255;
    g = (color>>8) & 255;
    b = (color>>16) & 255;
    a = (color>>24) & 255;
}

void save_ppm(const std::string filename, std::vector<uint32_t> image, const uint32_t w, const uint32_t h){
    std::ofstream ofs(filename, std::ios::binary);
    ofs<< "P6\n" << w << " " << h << "\n255\n";
    for(size_t i=0;i<image.size();i++){
        uint8_t r, g, b, a;
        unpack_color(image[i], r, g, b, a);
        ofs << static_cast<char>(r) << static_cast<char>(g) << static_cast<char>(b);
    }
    ofs.close();
}