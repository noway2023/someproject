#pragma once 

#include <cassert>
#include <iostream>
#include <vector>

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
    Vec2 & operator =(const Vec2 &v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
        }
        return *this;
    }

};

template <typename t>
class Vec3{
public:
    union 
    {
        struct{t x, y, z;};
        t raw[3];
    };

public:
    Vec3():x(0), y(0), z(0){}
    Vec3(t x1, t y1, t z1):x(x1), y(y1), z(z1){}
    Vec3(const Vec3& other):x(other.x), y(other.y), z(other.z){};
    
    t operator [] (size_t i) const{
        assert(i<2);
        return raw[i]; 
    }
    t operator * (Vec3 other){
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
    template <class > friend std::ostream& operator<<(std::ostream& s, Vec3<t>& v);

    float norm () const { return std::sqrt(x*x+y*y+z*z); }
	Vec3 & normalize(t l=1) { 
        // float n = l/norm();
        // y = y*n;
        // z = z*n;
        // x = x*n;
        // return *this; 
        *this = (*this)*(l/norm()); return *this;
    }
    Vec3 & operator =(const Vec3 &v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }


};

template <class t> 
std::ostream& operator<<(std::ostream& s, Vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template<typename t, size_t N=4 , size_t M=4>
class Matrix{
private:
    std::vector<std::vector<t>> data;

public:
    Matrix();
    static Matrix identity(size_t n);
    Matrix trans();
    Matrix inverse();

};