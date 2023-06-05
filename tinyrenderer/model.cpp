#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), vts_(), vns_(), faces_(), uv_(), normal_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f,texture,n;
            int normal, idx, uv;
            iss >> trash;
            while (iss >> idx >> trash >> uv >> trash >> normal) {
                idx--; // in wavefront obj all indices start at 1, not zero
                uv--;
                normal--;
                f.push_back(idx);
                texture.push_back(uv);
                n.push_back(normal);
            }
            uv_.push_back(texture);
            normal_.push_back(n);
            faces_.push_back(f);
        } else if(!line.compare(0, 4, "vt  ")){
            iss >> trash;
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            vts_.push_back(v);
        } else if(!line.compare(0, 4, "vn  ")){
            iss >> trash;
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            vns_.push_back(v);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
    std::cerr << "# vt# " << vts_.size() << " vn# " << vns_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

std::vector<int> Model::uvs(int idx) {
    return uv_[idx];
}

std::vector<int> Model::normals(int idx) {
    return normal_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec3f Model::vt(int i) {
    return vts_[i];
}

Vec3f Model::vn(int i, int j) {
    int idx = normal_[i][j];
    return vns_[idx].normalize();
}
