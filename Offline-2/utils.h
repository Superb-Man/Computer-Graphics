#pragma once
#include <bits/stdc++.h> 
#include <iomanip>
#include "bitmap_image.hpp"

class point {
    public:
    double x, y, z ;
    point() {

    }
    point(double x, double y , double z) {
        this->x = x ;
        this->y = y ;
        this->z = z ;
    }
    point operator*(double s) const{
        return point(x*s,y*s,z*s) ;
    }
    point operator/(double s) const{
        return point(x/s,y/s,z/s) ;
    }
    point operator+(const point& p) const{
        return point(x+p.x , y+p.y , z+p.z) ;
    }

    point operator-(const point& p) const{
        return point(x - p.x , y - p.y , z - p.z) ;
    }

};

point crossProduct(point u,point v) {
    return point(u.y * v.z - v.y * u.z ,
                    -u.x * v.z + v.x * u.z, u.x * v.y - u.y * v.x) ;
}

double dotProduct(point u, point v){
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

point normalize(point& p) {
    double length = sqrt(p.x * p.x + p.y * p.y + p.z * p.z) ;
    return point(p.x *1.0/ length , p.y *1.0/ length , p.z *1.0/ length) ;
}
struct Triangle{
    point a, b, c;
    rgb_t color;
};

//vector formate of line equation
struct Line{
    point u, v;
    //v for direction
};

//paralell of x axis(the second line)
std::pair<point,double> intersection_t(Line& l, double y){
    //l = u + tv
    // Y = y
    //u_y+t*v_y = y 
    double t = (y - l.u.y) / l.v.y;
    point p = l.u + l.v * t;

    return std::make_pair(p,t);
}

static unsigned long int g_seed = 1;
inline int random1()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

void print(Matrix& m ,int r , int c , bool col = false) {
    // std::cout << std::fixed << std::setprecision(4);
    // std::cout << "Matrix: " << m.getDimension().first << "x" << m.getDimension().second << std::endl;
    if(!col){
        for(int i = 0 ; i < r ; i++){
            for(int j = 0 ; j < c ; j++){
                std::cout << std::setprecision(9) ;
                std::cout << m.get(i,j) << " ";
            }
            std::cout << std::endl;
        }
    }
    else{
        for(int i = 0 ; i < c ; i++){
            for(int j = 0 ; j < r ; j++){
                std::cout << std::fixed <<std::setprecision(9) ;
                std::cout << m.get(j,i) << " ";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
}

