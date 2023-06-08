#pragma once

#include <fstream>
#include <iostream>
#include <vector>

uint32_t pack_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255);
void unpack_color(uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a);
void save_ppm(const std::string filename, std::vector<uint32_t> image, const uint32_t w, const uint32_t h);