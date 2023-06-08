#pragma once 

#include <cassert>
#include <iostream>

template <typename t>
class Vec2{
private:
    union
    {
        t x, y;
        t raw[2];
    };

public:
    
    Vec2():x(0), y(0){}
    Vec2(t x1, t y1):x(x1), y(y1){}
    Vec2(Vec2& other):x(other.x), y(other.y){};
    
    t operator [] (size_t i) const{
        assert(i<1);
        return raw[i]; 
    }
    t operator * (Vec2 &other){
        return x*other.x + y*other.y;
    }
    Vec2& operator * (t multi){
        return Vec2(x*multi, y*multi);
    }
    Vec2& operator - (Vec2& other){
        return Vec2(x-other.x, y-other.y);
    }
    Vec2& operator + (Vec2& other){
        return Vec2(x+other.x, y+other.y);
    }
    std::ostream operator << (std::ostream& os){
        os<< "(" << x << ", " << y << ")" << std::endl;
    }


};

template <typename t>
class Vec3{
private:
    union 
    {
        t x, y, z;
        t raw[3];
    };

public:
    Vec3():x(0), y(0), z(0){}
    Vec3(t x1, t y1, t z1):x(x1), y(y1), z(z1){}
    Vec3(Vec3& other):x(other.x), y(other.y), z(other.z){};
    
    t operator [] (size_t i) const{
        assert(i<2);
        return raw[i]; 
    }
    t operator * (Vec3 &other){
        return x*other.x + y*other.y + z*other.z;
    }
    Vec3& operator * (t multi){
        return Vec3(x*multi, y*multi, z*multi);
    }
    Vec3& operator - (Vec3& other){
        return Vec3(x-other.x, y-other.y, z-other.z);
    }
    Vec3& operator + (Vec3& other){
        return Vec3(x+other.x, y+other.y, z+other.z);
    }
    Vec3& operator ^ (Vec3& other){
        return Vec3(y*other.z-z*other.y, z*other.x-x*other.z, x*other.y-y*other.x);
    }
    std::ostream operator << (std::ostream& os){
        os<< "(" << x << ", " << y << "," << z << ")" << std::endl;
    }


};


typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template<typename t, size_t N=4 , size_t M=4>
class Matrix{
private:
    



};