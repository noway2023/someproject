#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include "utils.h"
#include "geometry.h"
#include <cassert>
#include <bitset>

const uint32_t red = 255;
const uint32_t green = 255<<8;
const uint32_t blue = 255<<16;
const uint32_t yellow = red + green;
const uint32_t white = red + blue + green;

struct Material {
    Material(const float r, const Vec4f &a, const uint32_t &color, const float spec) : refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
    Material() : refractive_index(1), albedo(1,0,0,0), diffuse_color(), specular_exponent() {}
    float refractive_index;
    Vec4f albedo;
    uint32_t diffuse_color;
    float specular_exponent;
};

class Sphere{
public:
    float radias;
    Vec3f center;
    Material material;
public:
    Sphere(Vec3f p, float r, Material m):center(p), radias(r), material(m){}

    bool isintersect(Vec3f &pos, Vec3f &dir, float &t0){
        Vec3f p = center - pos;
        float c = p*dir;
        float d = p*p - c*c;
        if(d > radias*radias) return false;
        float d2 = std::sqrt(radias*radias - d);
        t0 = c - d2;
        float t1 = c + d2;
        if(t0<0) t0 = t1;
        if(t0<0) return false;
        return true;
    }
};


Vec3f reflect(Vec3f eye, Vec3f normal){
    return eye - normal*2.*(eye * normal);

}

Vec3f refract(Vec3f eye, Vec3f normal, float frac = 0.9){
    float c = -normal*eye;
    float r = (c<0)?frac:1./frac;
    float k = 1 - r*r*(1-c*c);
    if(c<0){
        c = -c;
        normal = -normal;
        return (k<0)? Vec3f(0.,0.,0.):eye * r + normal*(r*c -std::sqrt(k));
    }
    else{
        return (k<0)? Vec3f(0.,0.,0.):eye * r + normal*(r*c -std::sqrt(k));
    }
}

bool scene_intersect(Vec3f &pos, Vec3f &dir, std::vector<Sphere> &sphere, Material &material,Vec3f &hit, Vec3f &normal){
    float sphere_dis = std::numeric_limits<float>::max();
    for(size_t i=0;i<sphere.size();i++){
        float dist_i;
        if(sphere[i].isintersect(pos, dir, dist_i) && dist_i < sphere_dis){
            sphere_dis = dist_i;
            hit = pos+ dir*dist_i;
            normal = (hit - sphere[i].center).normalize();
            material = sphere[i].material;
        }
    }

    float checkplane_dis = std::numeric_limits<float>::max();
    if(fabs(dir.y)>1e-3){
        float d = -(pos.y+4.)/dir.y;
        Vec3f point = pos+dir*d;
        if(d>0 && fabs(point.x)<10 && point.z<-10 && point.z>-30 && d < sphere_dis){
            hit = point;
            normal = Vec3f(0.,1.,0.);
            checkplane_dis = d;
            material.diffuse_color = (((int)(hit.x/2.+1000) +(int)(hit.z/2.)) &1)? blue + green:white;
        }
    }
    return std::min(sphere_dis,checkplane_dis)<1000;
}

uint32_t cast_ray(Vec3f &pos, Vec3f &dir, std::vector<Sphere> &sphere, std::vector<Vec3f> &lights, size_t depth=0){
    Material material;
    Vec3f normal, point;
    if(depth > 4 || !scene_intersect(pos, dir, sphere, material, point, normal)){
        return pack_color(0.2*255, 0.7*255, 0.8*255); 
    }

    Vec3f reflect_dir = reflect(dir, normal).normalize();
    Vec3f reflect_orig = reflect_dir*normal < 0 ? point - normal*1e-3 : point + normal*1e-3; 
    uint32_t reflect_color = cast_ray(reflect_orig, reflect_dir, sphere, lights, depth + 1);

    Vec3f refract_dir = refract(dir, normal, material.refractive_index).normalize();
    Vec3f refract_orig = refract_dir*normal < 0 ? point - normal*1e-3 : point + normal*1e-3; 
    uint32_t refract_color = cast_ray(refract_orig, refract_dir, sphere, lights, depth + 1);

    uint8_t fracr, fracg, fracb, fraca;
    unpack_color(refract_color, fracr, fracg, fracb, fraca);
    float fracrf = fracr/255., fracgf = fracg/255., fracbf = fracb/255.;
    uint8_t rr, rg, rb, ra;
    unpack_color(reflect_color, rr, rg, rb, ra);
    float rrf = rr/255., rgf = rg/255., rbf = rb/255.;
    uint8_t r, g, b, a;
    unpack_color(material.diffuse_color, r, g, b, a);
    float diffuse_light_intensity = 0,specular_light_intensity=0, shadow_intensity = 0;
    for (size_t i=0; i<lights.size(); i++) {
        Vec3f light_dir      = (lights[i] - point).normalize();

        Vec3f shadow_orig = light_dir*normal < 0 ? point - normal*1e-3 : point + normal*1e-3;
        Vec3f shadow_normal, shadow_point;
        float dis = (lights[i] - point).norm();
        Material temp;
        if(scene_intersect(shadow_orig, light_dir, sphere, temp, shadow_point, shadow_normal) && (shadow_point - shadow_orig).norm()<dis){
            continue;
        }
        diffuse_light_intensity  += 1 * std::max(0.f, light_dir*normal);
        specular_light_intensity += powf(std::max(0.f, reflect(light_dir, normal)*dir), material.specular_exponent)*1;
    }
    
    float rf = r/255., gf = g/255., bf = b/255.;
    float diffuse_coff = diffuse_light_intensity*material.albedo[0];
    float spec_coff = specular_light_intensity*material.albedo[1];
    float reflect_coff = material.albedo[2];
    float refract_coff = material.albedo[3];
    rf = rf*diffuse_coff + 1.*spec_coff + rrf*reflect_coff + fracrf*refract_coff;
    gf = gf*diffuse_coff + 1.*spec_coff + rgf*reflect_coff + fracgf*refract_coff;
    bf = bf*diffuse_coff + 1.*spec_coff + rbf*reflect_coff + fracbf*refract_coff;
    float maxf = std::max(std::max(rf, bf),gf);
    if(maxf<=1.0) maxf = 1.0;
    r = (255 * std::max(0.f, std::min(1.f, rf/maxf)));
    g = (255 * std::max(0.f, std::min(1.f, gf/maxf)));
    b = (255 * std::max(0.f, std::min(1.f, bf/maxf)));
    return pack_color(r, g, b);

}

void render(std::vector<uint32_t>& image, std::vector<Sphere>& spheres, std::vector<Vec3f> lights, float &tanfov, uint32_t w, uint32_t h){
    #pragma omp parallel for
    for(size_t i=0;i<w;i++){
        for(size_t j=0;j<h;j++){
            
            float dx = (2*(i+0.5)/(float)w -1) * tanfov * w / (float)h;
            float dy = -(2*(j+0.5)/(float)h -1) * tanfov;
            Vec3f dir = Vec3f(dx, dy, -1).normalize();
            image[j*w+i] = cast_ray(Vec3f(0.,0.,0.), dir, spheres, lights);
        }
    }
}

int main(){
    constexpr uint32_t width = 1024;
    constexpr uint32_t height = 768;
    constexpr int fov = M_PI/2.;
    float tanfov = tan(fov/2.);
    std::vector<uint32_t> framebuffer(width*height, 0);

    Material      ivory(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), pack_color(0.4*255, 0.4*255, 0.3*255),   50.);
    Material      glass(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), pack_color(0.6*255, 0.7*255, 0.8*255),  125.);
    Material red_rubber(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), pack_color(0.3*255, 0.1*255, 0.1*255),   10.);
    Material     mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), pack_color(1.0*255, 1.0*255, 1.0*255), 1425.);
    std::vector<Sphere> sp = {
        Sphere(Vec3f(-3.,    0.,   -16.), 2, ivory),
        Sphere(Vec3f(-1.0, -1.5, -12.), 2, glass),
        Sphere(Vec3f(1.5, -0.5, -18.), 3, red_rubber),
        Sphere(Vec3f(7.,    5.,   -18.), 4, mirror)
    };
    std::vector<Vec3f> light = {
        Vec3f(-20., 20., 20.),
        Vec3f( 30., 50., -25.),
        Vec3f( 30., 20.,  30.)
    };


    render(framebuffer, sp, light, tanfov, width, height);
    

    save_ppm("out.ppm", framebuffer, width, height);
    return 0;
}
